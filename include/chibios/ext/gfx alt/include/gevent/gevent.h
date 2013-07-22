/*
    ChibiOS/GFX - Copyright (C) 2012
                 Joel Bodenmann aka Tectu <joel@unormal.org>

    This file is part of ChibiOS/GFX.

    ChibiOS/GFX is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/GFX is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/**
 * @file    include/gevent/gevent.h
 * @brief   GEVENT GFX User Event subsystem header file.
 *
 * @addtogroup GEVENT
 *
 * @details		GEVENT provides a simple to use but yet powerful event
 *				system.
 *
 * @pre			GFX_USE_GEVENT must be set to TRUE in your gfxconf.h
 *
 * @{
 */
#ifndef _GEVENT_H
#define _GEVENT_H

#include "gfx.h"

#if GFX_USE_GEVENT || defined(__DOXYGEN__)

/* Data part of a static GListener initializer */
#define _GLISTENER_DATA(name) { _SEMAPHORE_DATA(name.waitqueue, 0), _BSEMAPHORE_DATA(name.eventlock, FALSE), 0, 0, {0} }
/* Static GListener initializer */
#define GLISTENER_DECL(name) GListener name = _GLISTENER_DATA(name)

/*===========================================================================*/
/* Type definitions                                                          */
/*===========================================================================*/

typedef uint16_t						GEventType;
		#define GEVENT_NULL				0x0000				// Null Event - Do nothing
		#define GEVENT_EXIT				0x0001				// The listener is being forced to exit (someone is destroying the listener)
		
		/* Other event types are allocated in ranges in their respective include files */
		#define GEVENT_GINPUT_FIRST		0x0100				// GINPUT events range from 0x0100 to 0x01FF
		#define GEVENT_GWIN_FIRST		0x0200				// GWIN events range from 0x0200 to 0x02FF
		#define GEVENT_GADC_FIRST		0x0300				// GADC events range from 0x0300 to 0x033F
		#define GEVENT_GAUDIN_FIRST		0x0340				// GAUDIN events range from 0x0340 to 0x037F
		#define GEVENT_GAUDOUT_FIRST	0x0380				// GAUDOUT events range from 0x0380 to 0x03BF
		#define GEVENT_USER_FIRST		0x8000				// Any application defined events start at 0x8000

// This object can be typecast to any GEventXxxxx type to allow any sub-system (or the application) to create events.
//	The prerequisite is that the new status structure type starts with a field named 'type' of type 'GEventType'.
//	The total status structure also must not exceed GEVENT_MAXIMUM_SIZE bytes.
//	For example, this is used by GWIN button events, GINPUT data streams etc.
typedef union GEvent_u {
	GEventType			type;								// The type of this event
	char				pad[GEVENT_MAXIMUM_SIZE];			// This is here to allow static initialisation of GEventObject's in the application.
	} GEvent;

// A special callback function
typedef void (*GEventCallbackFn)(void *param, GEvent *pe);

// The Listener Object
typedef struct GListener {
	Semaphore			waitqueue;			// Private: Semaphore for the listener to wait on.
	BinarySemaphore		eventlock;			// Private: Protect against more than one sources trying to use this event lock at the same time
	GEventCallbackFn	callback;			// Private: Call back Function
	void				*param;				// Private: Parameter for the callback function.
	GEvent				event;				// Public:  The event object into which the event information is stored.
	} GListener;

// The Source Object
typedef struct GSource_t			GSource, *GSourceHandle;	

// This structure is passed to a source to describe a contender listener for sending the current event.
typedef struct GSourceListener_t {
	GListener		*pListener;			// The listener
	GSource			*pSource;			// The source
	unsigned		listenflags;		// The flags the listener passed when the source was assigned to it.
	unsigned		srcflags;			// For the source's exclusive use. Initialised as 0 for a new listener source assignment.
	} GSourceListener;

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/* How to listen for events (act as a Listener)...
	1. Get handles for all the event sources you are interested in.
	2. Initialise a listener
	3. Attach sources to your listener.
		- Sources can be attached or detached from a listener at any time.
		- A source can be attached to more than one listener.
	4. Loop on getting listener events
	5. When finished detach all sources from the listener
	
	How to create events (act as a Source)...
	1. Provide a funtion to the application that returns a GSourceHandle (which can be a pointer to whatever the source wants)
	2. Whenever a possible event occurs call geventGetSourceListener to get a pointer to a GSourceListener.
			This will return NULL when there are no more listeners.
			For each listener	- check the flags to see if an event should be sent.
								- use geventGetEvent() to get the event buffer supplied by the listener
									and then call geventSendEvent to send the event.
								- Note: geventGetEvent() may return FALSE to indicate the listener is currently not listening and
									therefore no event should be sent. This situation enables the source to (optionally) flag
									to the listener on its next wait that there have been missed events.
								- Note: The GSourceListener pointer (and the GEvent buffer) are only valid between
									the geventGetSourceListener call and either the geventSendEvent call or the next
									geventGetSourceListener call.
								- Note: All listeners must be processed for this event before anything else is processed.
*/

/*---------- Listener Functions --------------------------------------------*/

/**
 * @brief	Create a Listener
 * @details	If insufficient resources are available it will either assert or return NULL
 *			depending on the value of GEVENT_ASSERT_NO_RESOURCE.
 *
 * @param[in] pl	A listener
 */
void geventListenerInit(GListener *pl);

/**
 * @brief 	Attach a source to a listener
 * @details	Flags are interpreted by the source when generating events for each listener.
 *			If this source is already assigned to the listener it will update the flags.
 *			If insufficient resources are available it will either assert or return FALSE
 *			depending on the value of GEVENT_ASSERT_NO_RESOURCE.
 *
 * @param[in] pl	The listener
 * @param[in] gsh	The source which has to be attached to the listener
 * @param[in] flags	The flags
 *
 * @return TRUE if succeeded, FALSE otherwise
 */
bool_t geventAttachSource(GListener *pl, GSourceHandle gsh, unsigned flags);

/**
 * @brief	Detach a source from a listener
 * @details	If gsh is NULL detach all sources from this listener and if there is still
 *			a thread waiting for events on this listener, it is sent the exit event.
 *
 * @param[in] pl	The listener
 * @param[in] gsh	The source
 */
void geventDetachSource(GListener *pl, GSourceHandle gsh);

/**
 * @brief	Wait for an event on a listener from an assigned source.
 * @details	The type of the event should be checked (pevent->type) and then pevent should
 *			be typecast to the actual event type if it needs to be processed.
 * 			timeout specifies the time to wait in system ticks.
 *			TIME_INFINITE means no timeout - wait forever for an event.
 *			TIME_IMMEDIATE means return immediately
 * @note	The GEvent buffer is staticly allocated within the GListener so the event does not
 *			need to be dynamicly freed however it will get overwritten by the next call to
 *			this routine.
 *
 * @param[in] pl		The listener
 * @param[in] timeout	The timeout
 *
 * @return	NULL on timeout
 */
GEvent *geventEventWait(GListener *pl, systime_t timeout);

/* @brief	Register a callback for an event on a listener from an assigned source.
 * @details	The type of the event should be checked (pevent->type) and then pevent should be typecast to the
 *			actual event type if it needs to be processed.
 *
 * @params[in] pl		The Listener
 * @params[in] fn		The function to call back
 * @params[in] param	A parameter to pass the callback function
 *
 * @note	The GEvent buffer is valid only during the time of the callback. The callback MUST NOT save
 * 			a pointer to the buffer for use outside the callback.
 * @note	An existing callback function is de-registered by passing a NULL for 'fn'. Any existing
 * 			callback function is replaced. Any thread currently waiting using geventEventWait will be sent the exit event.
 * @note	Callbacks occur in a thread context but stack space must be kept to a minumum and
 * 			the callback must process quickly as all other events are performed on a single thread.
 * @note	In the callback function you should never call ANY event functions using your own GListener handle
 * 			as it WILL create a deadlock and lock the system up.
 * @note	Applications should not use this call - geventEventWait() is the preferred mechanism for an
 * 			application. This call is provided for GUI objects that may not have their own thread.
 */
void geventRegisterCallback(GListener *pl, GEventCallbackFn fn, void *param);

/*---------- Source Functions --------------------------------------------*/

/**
 * @brief	Called by a source with a possible event to get a listener record.
 * @details	@p lastlr should be NULL on the first call and thereafter the result of the previous call.
 *
 * @param[in] gsh		The source handler
 * @param[in] lastlr	The source listener
 *
 * @return	NULL when there are no more listeners for this source
 */
GSourceListener *geventGetSourceListener(GSourceHandle gsh, GSourceListener *lastlr);

/**
 * @brief	Get the event buffer from the GSourceListener.
 * @details	A NULL return allows the source to record (perhaps in glr->scrflags) that the listener
 *			has missed events. This can then be notified as part of the next event for the listener.
 *			The buffer can only be accessed untill the next call to geventGetSourceListener
 *			or geventSendEvent
 *
 * @param[in] psl	The source listener
 *
 * @return	NULL if the listener is not currently listening.
 */
GEvent *geventGetEventBuffer(GSourceListener *psl);

/** 
 * @brief	Called by a source to indicate the listener's event buffer has been filled.
 * @details	After calling this function the source must not reference in fields in the GSourceListener or the event buffer.
 *
 * @param[in] psl	The source listener
 */
void geventSendEvent(GSourceListener *psl);

/**
 * @brief	Detach any listener that has this source attached
 *
 * @param[in] gsh	The source handle
 */
void geventDetachSourceListeners(GSourceHandle gsh);

#ifdef __cplusplus
}
#endif

#endif /* GFX_USE_GEVENT */

#endif /* _GEVENT_H */
/** @} */

