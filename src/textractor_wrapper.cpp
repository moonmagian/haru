#include "textractor_wrapper.h"
#include <QTextCodec>
#include <iostream>
textractor_wrapper::textractor_wrapper(QObject *parent)
    : QObject(parent), process(this),
      hook_pattern(R"(^(\[.*?]) (.*)$)", QRegularExpression::MultilineOption) {
#ifdef USE_WINE
    QString process_name{"wine"};
    QStringList arguments;
    arguments << "libs/textractor/TextractorCLI.exe";
#else
    QString process_name{"libs/textractor/TextractorCLI.exe"};
    QStringList arguments;
#endif
    connect(&process, &QProcess::readyReadStandardOutput, this,
            &textractor_wrapper::on_process_data_ready);
    process.start(process_name, arguments);
    process.waitForStarted(3000);
    if (process.state() != QProcess::Running) {
        qCritical("Can't start Textractor.");
        return;
    }
    qInfo("Started Textractor.");
}

textractor_wrapper::~textractor_wrapper() {
    if (process.state() == QProcess::Running) {
        process.terminate();
    }
}

bool textractor_wrapper::attach(unsigned int pid) {
    if (process.state() != QProcess::Running) {
        qCritical("Textractor is not started.");
        return false;
    }
    QString request = QString("attach -P%1\n").arg(pid);
    QTextCodec *codec = QTextCodec::codecForName("UTF-16");
    QTextEncoder *encoder = codec->makeEncoder(QTextCodec::IgnoreHeader);

    QByteArray bytes = encoder->fromUnicode(request);
    if (process.write(bytes) == -1) {
        qCritical("Can't write attach information to textractor.");
        return false;
    }
    qInfo("Written attach information to textractor.");
    return true;
}

void textractor_wrapper::terminate() { process.kill(); }

void textractor_wrapper::on_process_data_ready() {
    QTextCodec *codec = QTextCodec::codecForName("UTF-16");
    QTextDecoder *decoder = codec->makeDecoder(QTextCodec::IgnoreHeader);
    QString str = decoder->toUnicode(process.readAllStandardOutput());
    //    qDebug("%s", str.toStdString().c_str());
    auto str_split = str.split(QRegExp("[\r\n]"), Qt::SkipEmptyParts);
    QString name = "";
    QString value = "";
    for (const auto &s : str_split) {
        auto result = hook_pattern.match(s);
        if (result.hasMatch()) {
            if (name != "") {
                emit updated_hook_text(name, value);
                //                qDebug("%s", value.toStdString().c_str());
            }
            name = result.captured(1);
            value = result.captured(2);
        } else {
            value += "\n";
            value += s;
        }
    }
    if (name != "") {
        emit updated_hook_text(name, value);
        qDebug("%s", value.toStdString().c_str());
    }
    delete decoder;
}
