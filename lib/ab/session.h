/***************************************************************************
 *   Copyright (C) 2015 by OmanTek                                         *
 *   Author Kyle Hayes  kylehayes@omantek.com                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef __AB_SESSION_H__
#define __AB_SESSION_H__ 1

#include <libplctag.h>
#include <libplctag_tag.h>
#include <platform.h>

#define MAX_REQ_RESP_SIZE	(600) /* enough? */


/* struct to hold session information */
typedef struct ab_session_t *ab_session_p;
typedef struct ab_request_t *ab_request_p;
typedef struct ab_connection_t *ab_connection_p; /* to get around circular .h includes */

struct ab_session_t {
	ab_session_p next;
	ab_session_p prev;

	/* gateway connection related info */
	char host[MAX_SESSION_HOST];
	int port;
	sock_p sock;

	int debug;
    
    lock_t lock;
	//int is_connected;
	int status;

	/* registration info */
	uint32_t session_handle;

	/* Sequence ID for requests. */
	uint64_t session_seq_id;

	/* current request being sent, only one at a time */
	ab_request_p current_request;

	/* list of outstanding requests for this session */
	ab_request_p requests;

	/* counter for number of messages in flight */
	//int num_reqs_in_flight;

	/* data for receiving messages */
	uint64_t resp_seq_id;
	int has_response;
	int recv_offset;
	uint8_t recv_data[MAX_REQ_RESP_SIZE];

	/* tags for this session */
	int tag_count;
    
    /* state for the state machine */
    int state;

	/* connections for this session */
	ab_connection_p connections;
    
	uint32_t conn_serial_number; /* id for the next connection */
};

extern ab_session_p session_find_or_add(plc_tag tag);
extern void session_inc_tag_count(ab_session_p session);
extern void session_dec_tag_count(ab_session_p session);

/*
 * this structure contains data necessary to set up a request and hold
 * the resulting response.
 */

struct ab_request_t {
	ab_request_p next; 	/* for linked list */

	int req_id; 		/* which request is this for the tag? */
	int data_size; 		/* how many bytes did we get? */

	/* flags for communicating with background thread */
	int send_request;
	int send_in_progress;
	int resp_received;
	int recv_in_progress;
	int abort_request;
	int abort_after_send; /* for one shot packets */

	int status;
	int debug;

	/* used when processing a response */
	int processed;

	ab_session_p session;

	uint64_t session_seq_id;
	uint32_t conn_id;
	uint16_t conn_seq;

	/* used by the background thread for incrementally getting data */
	int current_offset;
	int request_size; /* total bytes, not just data */
	uint8_t data[MAX_REQ_RESP_SIZE];
};


int request_create(ab_request_p *req);
int request_add(ab_session_p sess, ab_request_p req);
int request_remove(ab_session_p sess, ab_request_p req);
int request_destroy(ab_request_p *req);


#endif
