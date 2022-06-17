#include <Windows.h>
#include <QDebug>


int main(int argc, char *argv[])
{

       DWORD pid = (DWORD) atoi(argv[1]); // получает из аргументов PID процесса, который надо защищать

       bool isAttached = DebugActiveProcess(pid); // подключаемся к нему как отладчик

       if(!isAttached) { // Проверяем, удалось ли подключиться
           DWORD lastError = GetLastError();
           qDebug() << "GetLastError()" << lastError;
           return lastError;
       }

       DEBUG_EVENT DbgEvent;

       while(1) { // бесконечный цикл ожидания событий отладки от менеджера паролей
           WaitForDebugEvent(&DbgEvent, INFINITE);
           ContinueDebugEvent(DbgEvent.dwProcessId, DbgEvent.dwThreadId, DBG_CONTINUE);
       }
}
