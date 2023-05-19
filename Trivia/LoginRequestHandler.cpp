#include "LoginRequestHandler.h"
#include "SqliteDatabase.h"

LoginRequestHandler::LoginRequestHandler(RequestHandlerFactory& handlerFactory) :
    m_handlerFactory(handlerFactory), m_loginManager(m_handlerFactory.getLoginManager())
{

}

bool LoginRequestHandler::isRequestRelevant(RequestInfo reqInfo)
{
    return ((reqInfo.id == LOGIN_REQUEST) || (reqInfo.id == SIGNUP_REQUEST));
}

RequestResult LoginRequestHandler::handleRequest(RequestInfo reqInfo)
{
    /*
    * function desrialize client request 
    * then checks if request is valid Login Or signUp
    * and if posibble to login or signUp using the Data from the request
    * like if login so the userName and Password should be in our data base
    * and signUp need to be check for his data.
    */
    RequestResult reqResult;
    ErrorRespone errorRes;

    if (isRequestRelevant(reqInfo))
    {
        if (reqInfo.id == LOGIN_REQUEST)
        {
            reqResult = login(reqInfo);
        }
        else if (reqInfo.id == SIGNUP_REQUEST)
        {
            reqResult = signup(reqInfo);
        }
    }
    else // code does not exist
    {
        errorRes.message = ERROR_CODE_MSG;
        reqResult.buffer = JsonResponsePacketSerializer::serializeRespone(errorRes);
        reqResult.newHandler = nullptr;
    }
    return reqResult;
}

/*
Function get login request and return login respone
*/
RequestResult LoginRequestHandler::login(RequestInfo reqInfo)
{
    LoginRequest loginRequest = JsonRequestPacketDeserializer::deserializeLoginRequest(reqInfo.buffer);
    LoginRespone loginRes;
    RequestResult reqResult;

    if (m_loginManager.login(loginRequest.username, loginRequest.password) == LOGIN_SUCCESS_STATUS)
    {
        loginRes.status = LOGIN_SUCCESS_STATUS;
        reqResult.newHandler = m_handlerFactory.createMenuRequestHandler(loginRequest.username);
    }
    else
    {
        loginRes.status = LOGIN_FAILURE_STATUS;
        reqResult.newHandler = m_handlerFactory.createLoginRequestHandler();
    }
    reqResult.buffer = JsonResponsePacketSerializer::serializeRespone(loginRes);
    return reqResult;
}

/*
Function get signup request and return signup respone
*/
RequestResult LoginRequestHandler::signup(RequestInfo reqInfo)
{
    SignUpRequest signUpRequest = JsonRequestPacketDeserializer::deserializeSignUpRequest(reqInfo.buffer);
    SignupRespone signupRes;
    RequestResult reqResult;
    if (m_loginManager.signup(signUpRequest.username, signUpRequest.password, signUpRequest.email) == SIGNUP_SUCCESS_STATUS)
    {
        signupRes.status = SIGNUP_SUCCESS_STATUS;
    }
    else
    {
        signupRes.status = SIGNUP_FAILURE_STATUS;
    }
    reqResult.newHandler = m_handlerFactory.createLoginRequestHandler();
    reqResult.buffer = JsonResponsePacketSerializer::serializeRespone(signupRes);
    return reqResult;
}
