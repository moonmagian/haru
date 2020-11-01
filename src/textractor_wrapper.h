#ifndef TEXTRACTOR_WRAPPER_H
#define TEXTRACTOR_WRAPPER_H
#include <QObject>
#include <QProcess>
#include <QRegularExpression>
class textractor_wrapper : public QObject {
    Q_OBJECT
    public:
    explicit textractor_wrapper(QObject *parent = nullptr);
    ~textractor_wrapper() override;
    bool attach(unsigned int pid);
    void terminate();

    private:
    QProcess process;
    QRegularExpression hook_pattern;
    private slots:
    void on_process_data_ready();
    signals:
    void updated_hook_text(QString hookname, QString value);
};

#endif // TEXTRACTOR_WRAPPER_H
