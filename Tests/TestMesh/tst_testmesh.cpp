#include <QtTest>

// add necessary includes here

class TestMesh : public QObject
{
    Q_OBJECT

public:
    TestMesh();
    ~TestMesh();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();

};

TestMesh::TestMesh()
{

}

TestMesh::~TestMesh()
{

}

void TestMesh::initTestCase()
{

}

void TestMesh::cleanupTestCase()
{

}

void TestMesh::test_case1()
{

}

QTEST_APPLESS_MAIN(TestMesh)

#include "tst_testmesh.moc"
