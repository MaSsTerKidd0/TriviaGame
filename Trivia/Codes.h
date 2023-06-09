#pragma once

// Requests
// ----------
#define SIGNUP_REQUEST 10
#define LOGIN_REQUEST 20
#define CREATE_ROOM_REQUEST 30
#define GET_ROOMS_REQUEST 40
#define GET_PLAYERS_IN_ROOM_REQUEST 50
#define JOIN_ROOM_REQUEST 60
#define GET_PERSONAL_STATISTICS_REQUEST 70
#define GET_BEST_STATISTICS_REQUEST 80
#define LOGOUT_REQUEST 90
#define ROOM_INFO_REQUEST 110
#define CLOSE_ROOM_REQUEST 120
#define START_GAME_REQUEST 130
#define GET_ROOM_STATE_REQUEST 140
#define LEAVE_ROOM_REQUEST 150

// Respones
// --------
#define ERROR_RESPONE_CODE 200 // CODE DOES NOT EXIST

#define SIGNUP_RESPONE 11
#define LOGIN_RESPONE 22
#define CREATE_ROOM_RESPONE 33
#define GET_ROOMS_RESPONE 44
#define GET_PLAYERS_IN_ROOM_RESPONE 55
#define JOIN_ROOM_RESPONE 66
#define GET_PERSONAL_STATISTICS_RESPONE 77
#define GET_BEST_STATISTICS_RESPONE 88
#define LOGOUT_RESPONE 99
#define ROOM_INFO_RESPONE 111
#define CLOSE_ROOM_RESPONE 122
#define START_GAME_RESPONE 133
#define GET_ROOM_STATE_RESPONE 144
#define LEAVE_ROOM_RESPONE 155

// Statuses
// --------
#define SIGNUP_SUCCESS_STATUS 1
#define SIGNUP_FAILURE_STATUS 201

#define LOGIN_SUCCESS_STATUS 2
#define LOGIN_FAILURE_STATUS 202

#define CREATE_ROOM_SUCCESS_STATUS 3
#define CREATE_ROOM_FAILURE_STATUS 203

#define GET_ROOMS_SUCCESS_STATUS 4
#define GET_ROOMS_FAILURE_STATUS 204

#define GET_PLAYERS_IN_ROOM_SUCCESS_STATUS 5
#define GET_PLAYERS_IN_ROOM_FAILURE_STATUS 205

#define JOIN_ROOM_SUCCESS_STATUS 6
#define JOIN_ROOM_FAILURE_STATUS 206

#define GET_PERSONAL_STATISTICS_SUCCESS_STATUS 7
#define GET_PERSONAL_STATISTICS_FAILURE_STATUS 207

#define GET_BEST_STATISTICS_SUCCESS_STATUS 8
#define GET_BEST_STATISTICS_FAILURE_STATUS 208

#define LOGOUT_SUCCESS_STATUS 9
#define LOGOUT_FAILURE_STATUS 209

#define CLOSE_ROOM_SUCCESS_STATUS 240
#define START_GAME_SUCCESS_STATUS 241
#define GET_ROOM_STATE_SUCCESS_STATUS 242
#define LEAVE_ROOM_SUCCESS_STATUS 243

// Error Messages
// --------------
#define ERROR_CODE_MSG "Code Does Not Exist!"