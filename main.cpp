#include <iostream>
#include <string>
#include <python3.8/Python.h>
#include <QApplication>
#include <QFormLayout>
#include <QtGlobal>
#include <QObject>
#include <QSlider>
#include <QSpinBox>
#include <QWidget>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    auto window = new QWidget;
    auto spin_box = new QSpinBox;
    spin_box->setRange(0, 100);
    auto slider = new QSlider;
    slider->setRange(0, 100);
    QObject::connect(spin_box, QOverload<int>::of(&QSpinBox::valueChanged), slider, &QSlider::setValue);
    QObject::connect(slider, &QSlider::valueChanged, spin_box, &QSpinBox::setValue);
    auto layout = new QFormLayout;
    layout->addRow("spin box:", spin_box);
    layout->addRow("slider", slider);
    window->setLayout(layout);
    window->show();

    Py_Initialize();
    std::string py_path("./");
    std::string chdir_cmd("sys.path.append(\"");
    chdir_cmd += py_path;
    chdir_cmd += "\")";
    std::cout << "chdir_cmd is :" <<chdir_cmd <<  std::endl;
    PyRun_SimpleString("import sys");
    PyRun_SimpleString(chdir_cmd.c_str());

//    PyObject *module_name = PyBytes_FromString("hello");
    PyObject *p_moudle = PyImport_ImportModule("hello");
    if (!p_moudle)
    {
        std::cout << "python get module failed!!!" << std::endl;

    }
    PyObject *p_func = PyObject_GetAttrString(p_moudle, "py_add");
    if (!p_func || !PyCallable_Check(p_func))
    {
        std::cout << " can't find function >py_add<" << std::endl;
    }
    PyObject *args = PyTuple_New(2);
    PyObject *arg1 = PyLong_FromLong(1000);
    PyObject *arg2 = PyLong_FromLong(2);
    PyTuple_SetItem(args, 0, arg1);
    PyTuple_SetItem(args, 1, arg2);

    PyObject *p_ret = PyObject_CallObject(p_func, args);
    if (p_ret)
    {
        long result = PyLong_AsLong(p_ret);
        std::cout << "py result: " << result;
    }
    Py_Finalize();

    return app.exec();
}
