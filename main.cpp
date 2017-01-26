#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlListProperty>
#include <QList>
#include <QVector>
#include <QDebug>

struct MyGadget
{
    Q_GADGET
public:
    int a;
    int b;
};

QDebug operator<<(QDebug out, const MyGadget& gadget)
{
    return out << gadget.a << gadget.b;
}

QDebug operator<<(QDebug out, const MyGadget* gadget)
{
    return out << *gadget;
}

Q_DECLARE_METATYPE(MyGadget)

class MyObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<QObject> objectList READ objectList)
    Q_PROPERTY(QQmlListProperty<MyGadget> gadgetList READ gadgetList)
    Q_PROPERTY(QQmlListProperty<MyGadget> gadgetVector READ gadgetVector)

public:
    explicit MyObject(QObject *parent = nullptr)
        : QObject(parent)
    {
        {
            auto foo = new QObject;
            foo->setObjectName("foo");
            auto bar = new QObject;
            bar->setObjectName("bar");
            m_objects << foo << bar;
        }
        m_gadgets << new MyGadget{1, 2} << new MyGadget{3, 4};
        m_gadgetVector << MyGadget{1, 2} << MyGadget{3, 4};
    }

    ~MyObject()
    {
        qDeleteAll(m_objects);
        qDeleteAll(m_gadgets);
    }

    QQmlListProperty<QObject> objectList()
    {
        return QQmlListProperty<QObject>(this, m_objects);
    }

    QQmlListProperty<MyGadget> gadgetList()
    {
        return QQmlListProperty<MyGadget>(this, m_gadgets);
    }

    QQmlListProperty<MyGadget> gadgetVector()
    {
        auto count = [] (QQmlListProperty<MyGadget>* p) {
            return reinterpret_cast<QVector<MyGadget>*>(p->data)->count();
        };
        auto at = [] (QQmlListProperty<MyGadget>* p, int idx) {
            return reinterpret_cast<QVector<MyGadget>*>(p->data)->data() + idx;
        };

        return QQmlListProperty<MyGadget>(this, &m_gadgetVector, count, at);
    }

private:
    QList<QObject*> m_objects;
    QList<MyGadget*> m_gadgets;
    QVector<MyGadget> m_gadgetVector;
};

Q_DECLARE_METATYPE(QQmlListProperty<MyGadget>)

template<typename T>
void printList(const char* name, QQmlListProperty<T> list)
{
    qWarning() << name;
    const int count = list.count(&list);
    qDebug() << "count" << count;
    for (int i = 0; i < count; ++i) {
        qDebug() << i << list.at(&list, i);
    }
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<MyObject>("foo.bar", 1, 0, "MyObject");
    qmlRegisterUncreatableType<MyGadget>("foo.bar", 1, 0, "MyGadget", "gadget only");

    auto myObject = new MyObject(&app);
    printList("objectList", myObject->objectList());
    printList("gadgetList", myObject->gadgetList());
    printList("gadgetVector", myObject->gadgetVector());

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("myObject", myObject);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

#include "main.moc"
