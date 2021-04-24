#include "translating_worker_python.h"
#include <QTextCodec>
//#include "lua_worker_http_extension.h"

translating_worker_python::translating_worker_python() {}

void translating_worker_python::init() {}

void translating_worker_python::run() {
    mutex.lock();
    QProcess python_proc;
#ifdef USE_WINE
    python_proc.setProgram("python");
#else
    python_proc.setProgram("python.exe");
#endif
    QStringList parameters;
    parameters << path;
    python_proc.setArguments(parameters);
    python_proc.start();
    python_proc.waitForStarted(3000);
    if (python_proc.state() != QProcess::Running) {
        qCritical("Python plugin %s is not running!",
                  path.toStdString().c_str());
        mutex.unlock();
        return;
    }
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextEncoder *encoder = codec->makeEncoder(QTextCodec::IgnoreHeader);
    auto encoded = encoder->fromUnicode(text);
    delete encoder;
    python_proc.write(encoded);
    qDebug("Written data to python plugin %s.", path.toStdString().c_str());
    if (!python_proc.waitForBytesWritten(3000)) {
        qCritical("Can't write data to python plugin %s.",
                  path.toStdString().c_str());
        mutex.unlock();
        return;
    }
    python_proc.closeWriteChannel();
    python_proc.waitForFinished(-1);
    // python_proc.waitForReadyRead(-1);
    QTextDecoder *decoder = codec->makeDecoder(QTextCodec::IgnoreHeader);
    QString result = decoder->toUnicode(python_proc.readAllStandardOutput());
    delete decoder;
    emit translation_done(result);
    mutex.unlock();
    return;
}

translating_worker_python::~translating_worker_python() {}
