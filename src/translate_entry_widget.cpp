#include "translate_entry_widget.h"
#include "ui_translate_entry_widget.h"
#include "translating_worker_lua.h"
#include <iostream>
#include <lua.hpp>
#include <lualib.h>
#include <lauxlib.h>
#include <QGraphicsDropShadowEffect>
translate_entry_widget::translate_entry_widget(QWidget *parent)
    : QWidget(parent), ui(new Ui::translate_entry_widget),
      worker(new translating_worker_lua) {
    ui->setupUi(this);
    this->setLayout(ui->horizontalLayout);
    //    connect(this, &translate_entry_widget::execution_finished, this,
    //            &translate_entry_widget::update_execution_result);
    worker->connect(worker, &translating_worker::translation_done, this,
                    &translate_entry_widget::update_execution_result);
    freshness = 0;
    QGraphicsDropShadowEffect *dse = new QGraphicsDropShadowEffect(this);
    dse->setBlurRadius(5);
    dse->setColor(QColor(255, 192, 203));
    dse->setXOffset(0);
    dse->setYOffset(0);
    ui->content->setGraphicsEffect(dse);
}

translate_entry_widget::~translate_entry_widget() {
    delete worker;
    delete ui;
}

void translate_entry_widget::set_name(const QString &name) {
    ui->name->setText(name);
}

void translate_entry_widget::set_script(const QString &script) {
    this->script = script;
    worker->set_path(script);
}

QString translate_entry_widget::get_name() { return ui->name->text(); }

void translate_entry_widget::push_text(const QString &text) {
    current_text = text;
    ++freshness;
    if (!worker->isRunning()) {
        worker->set_freshness(freshness);
        worker->set_text(current_text);
        worker->start();
    }
}

void translate_entry_widget::update_execution_result(const QString &result) {
    // Out-dated result, drop it.
    if (freshness != worker->get_freshness()) {
        if (!worker->isRunning()) {
            worker->set_freshness(freshness);
            worker->set_text(current_text);
            worker->start();
        }
    } else {
        ui->content->setText(result);
    }
}

// void translate_entry_widget::execute_py(const QString &text) {
//    // Ugly python.
//    // Fuck.
//    long code = 0;
//    QString result;
//    std::string text_stdstr = text.toStdString();
//    std::string script_stdstr = script.toStdString();
//    PyObject *pName, *pModule, *pFunc;
//    PyObject *pArgs, *pValue, *pResultCode, *pResultStr;
//    std::wstring ws(Py_GetPath());
//    Py_Initialize();
//    pName = PyUnicode_DecodeFSDefault(script_stdstr.c_str());
//    pModule = PyImport_Import(pName);
//    Py_DECREF(pName);
//    if (pModule != nullptr) {
//        pFunc = PyObject_GetAttrString(pModule, "translate");

//        if (pFunc && PyCallable_Check(pFunc)) {
//            pArgs = PyTuple_New(1);
//            pValue = PyUnicode_FromString(text_stdstr.c_str());
//            if (!pValue) {
//                Py_DECREF(pArgs);
//                Py_DECREF(pModule);
//                emit execution_finished(
//                    execution_codes::error,
//                    "[Error] Can't convert string to py string.");
//                return;
//            }
//            PyTuple_SetItem(pArgs, 0, pValue);
//            pValue = PyObject_CallObject(pFunc, pArgs);
//            Py_DECREF(pArgs);
//            if (pValue != nullptr) {
//                pResultCode = PyTuple_GetItem(pValue, 0);
//                code = PyLong_AsLong(pResultCode);
//                if (!pResultCode) {
//                    Py_DECREF(pValue);
//                    emit execution_finished(execution_codes::error,
//                                            "[Error] Can't get result code.");
//                    return;
//                }
//                pResultStr = PyTuple_GetItem(pValue, 1);
//                if (!pResultStr) {
//                    Py_DECREF(pValue);
//                    emit execution_finished(execution_codes::error,
//                                            "[Error] Can't get result
//                                            string.");
//                    return;
//                }
//                result = QString::fromUtf8(PyUnicode_AsUTF8(pResultStr));
//                Py_DECREF(pValue);
//            } else {
//                Py_DECREF(pFunc);
//                Py_DECREF(pModule);
//                PyErr_Print();
//                emit execution_finished(execution_codes::error,
//                                        "[Error] Can't call the function.");
//                return;
//            }
//        } else {
//            if (PyErr_Occurred()) {
//                PyErr_Print();
//            }
//            emit execution_finished(
//                execution_codes::error,
//                "[Error] Can't find the translate function.");
//            Py_XDECREF(pFunc);
//            Py_DECREF(pModule);
//        }
//    } else {
//        PyErr_Print();
//        emit execution_finished(execution_codes::error,
//                                "[Error] Can't load the script file.");
//        return;
//    }
//    if (Py_FinalizeEx() < 0) {
//        fprintf(stderr, "Failed to finalize python interpreter.\n");
//    }
//    std::cout << result.toStdString() << std::endl;
//    emit execution_finished(
//        code == 0 ? execution_codes::finished : execution_codes::error,
//        result);
//    return;
//}
