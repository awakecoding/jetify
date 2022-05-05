
#include "WSMan.h"

uint32_t WSManInitialize(uint32_t flags, WSMAN_API_HANDLE* apiHandle)
{
    return 0;
}

uint32_t WSManDeinitialize(WSMAN_API_HANDLE apiHandle, uint32_t flags)
{
    return 0;
}

uint32_t WSManGetErrorMessage(
    WSMAN_API_HANDLE apiHandle,
    uint32_t flags,
    const WCHAR* languageCode,
    uint32_t errorCode,
    uint32_t messageLength,
    WCHAR* message,
    uint32_t* messageLengthUsed)
{
    return 0;
}

uint32_t WSManCreateSession(WSMAN_API_HANDLE apiHandle, const WCHAR* connection,
    uint32_t flags, WSMAN_AUTHENTICATION_CREDENTIALS* serverAuthenticationCredentials,
    WSMAN_PROXY_INFO* proxyInfo, WSMAN_SESSION_HANDLE* session)
{
    return 0;
}

uint32_t WSManCloseSession(WSMAN_SESSION_HANDLE session, uint32_t flags)
{
    return 0;
}

uint32_t WSManSetSessionOption(WSMAN_SESSION_HANDLE session,
    WSManSessionOption option, WSMAN_DATA* data)
{
    return 0;
}

uint32_t WSManGetSessionOptionAsDword(
    WSMAN_SESSION_HANDLE session,
    WSManSessionOption option,
    uint32_t* value)
{
    return 0;
}

uint32_t WSManGetSessionOptionAsString(
    WSMAN_SESSION_HANDLE session,
    WSManSessionOption option,
    uint32_t stringLength,
    WCHAR* string,
    uint32_t* stringLengthUsed)
{
    return 0;
}

uint32_t WSManCloseOperation(WSMAN_OPERATION_HANDLE operationHandle, uint32_t flags)
{
    return 0;
}

void WSManSignalShell(
    WSMAN_SHELL_HANDLE shell,
    WSMAN_COMMAND_HANDLE command,
    uint32_t flags,
    const WCHAR* code,
    WSMAN_SHELL_ASYNC* async,
    WSMAN_OPERATION_HANDLE* signalOperation)
{
    
}

void WSManReceiveShellOutput(
    WSMAN_SHELL_HANDLE shell,
    WSMAN_COMMAND_HANDLE command,
    uint32_t flags,
    WSMAN_STREAM_ID_SET* desiredStreamSet,
    WSMAN_SHELL_ASYNC* async,
    WSMAN_OPERATION_HANDLE* receiveOperation)
{
    
}

void WSManSendShellInput(
    WSMAN_SHELL_HANDLE shell,
    WSMAN_COMMAND_HANDLE command,
    uint32_t flags,
    const WCHAR* streamId,
    WSMAN_DATA* streamData,
    BOOL endOfStream,
    WSMAN_SHELL_ASYNC* async,
    WSMAN_OPERATION_HANDLE* sendOperation)
{

}

void WSManCloseCommand(WSMAN_COMMAND_HANDLE commandHandle,
    uint32_t flags, WSMAN_SHELL_ASYNC* async)
{

}

void WSManCloseShell(WSMAN_SHELL_HANDLE shellHandle, uint32_t flags, WSMAN_SHELL_ASYNC* async)
{

}

void WSManCreateShellEx(
    WSMAN_SESSION_HANDLE session,
    uint32_t flags,
    const WCHAR* resourceUri,
    const WCHAR* shellId,
    WSMAN_SHELL_STARTUP_INFO* startupInfo,
    WSMAN_OPTION_SET* options,
    WSMAN_DATA* createXml,
    WSMAN_SHELL_ASYNC* async,
    WSMAN_SHELL_HANDLE* shell)
{

}

void WSManRunShellCommandEx(
    WSMAN_SHELL_HANDLE shell,
    uint32_t flags,
    const WCHAR* commandId,
    const WCHAR* commandLine,
    WSMAN_COMMAND_ARG_SET* args,
    WSMAN_OPTION_SET* options,
    WSMAN_SHELL_ASYNC* async,
    WSMAN_COMMAND_HANDLE* command)
{

}

void WSManDisconnectShell(WSMAN_SHELL_HANDLE shell, uint32_t flags,
    WSMAN_SHELL_DISCONNECT_INFO* disconnectInfo, WSMAN_SHELL_ASYNC* async)
{

}

void WSManReconnectShell(WSMAN_SHELL_HANDLE shell,
    uint32_t flags, WSMAN_SHELL_ASYNC* async)
{

}

void WSManReconnectShellCommand(WSMAN_COMMAND_HANDLE commandHandle, uint32_t flags, WSMAN_SHELL_ASYNC* async)
{

}

void WSManConnectShell(
    WSMAN_SESSION_HANDLE session,
    uint32_t flags,
    const WCHAR* resourceUri,
    const WCHAR* shellID,
    WSMAN_OPTION_SET* options,
    WSMAN_DATA* connectXml,
    WSMAN_SHELL_ASYNC* async,
    WSMAN_SHELL_HANDLE* shell)
{

}

void WSManConnectShellCommand(
    WSMAN_SHELL_HANDLE shell,
    uint32_t flags,
    const WCHAR* commandID,
    WSMAN_OPTION_SET* options,
    WSMAN_DATA* connectXml,
    WSMAN_SHELL_ASYNC* async,
    WSMAN_COMMAND_HANDLE* command)
{
    
}
