#include <QtTest>
#include "../geometry/datastructures.h"
#include "../geometry/arrangement.h"

class tst_Geometry : public QObject {
    Q_OBJECT
public:
    //=========================================================================
    //      Datastructures: Factories
    //=========================================================================
    DDAD::BinaryNode<int> SampleBinaryNode_int(){
        DDAD::BinaryNode<int> bnd = DDAD::BinaryNode<int>();
        bnd.left = nullptr;
        bnd.right = nullptr;
        bnd.element = 1;
        return bnd;
    }

    DDAD::SplayTree<int> SampleSplayTree_int(){
        DDAD::SplayTree<int> spt = DDAD::SplayTree<int>();
        int toinsert[] = {1, 2, 3, 4, 0};
        for(int ii = 0; ii < 5; ii++){
            spt.insert(toinsert[ii]);
        }
        return spt;
    }
    //=========================================================================
    //      Arrangement: Factories
    //=========================================================================
private slots:
    //=========================================================================
    //      Datastructures: Tests
    //=========================================================================
    void BinaryNodeConstructors(){
        DDAD::BinaryNode<int> tmpNode = SampleBinaryNode_int();
        QCOMPARE(tmpNode.getElement(), 1);

        // Try the copy constructor as well
        DDAD::BinaryNode<int> node2 = DDAD::BinaryNode<int>(2, &tmpNode, nullptr);
        QCOMPARE(node2.left->getElement(), 1);
    }


    void SplayTreeConstructors(){
        DDAD::SplayTree<int> spt = DDAD::SplayTree<int>();
        QVERIFY(spt.getRoot()==nullptr);

        spt = SampleSplayTree_int();
        DDAD::SplayTree<int> spt2 = spt;
        QCOMPARE(spt2.getRoot()->getElement(), spt.getRoot()->getElement());
    }

    void SplayTreeInserts(){
        DDAD::SplayTree<int> spt = SampleSplayTree_int();
        // Inserting an element should splay to the top
        spt.insert(20);
        QCOMPARE(spt.ContainsValue(20), true);
        QCOMPARE(spt.getRoot()->getElement(), 20);

    }
    void SplayTreeRemove(){
        DDAD::SplayTree<int> spt = SampleSplayTree_int();
        // Remove 2 from the tree
        spt.remove(2);
        // Tree should now remove 2 from the tree
        QCOMPARE(spt.ContainsValue(2), false);
    }
    void SplayTreeFind(){
        DDAD::SplayTree<int> spt = SampleSplayTree_int();
        // Find should splay an element to the top if it exists
        spt.find(3);
        QCOMPARE(spt.getRoot()->getElement(), 3);
        // If not, it should default to greatest elt less than
        spt.find(20);
        QCOMPARE(spt.getRoot()->getElement(), 4);
        // or least elt greater than
        spt.find(-1);
        QCOMPARE(spt.getRoot()->getElement(), 0);
    }
    void SplayTreeMerge(){
        DDAD::SplayTree<int> spt = SampleSplayTree_int();
        DDAD::SplayTree<int> spt2 = DDAD::SplayTree<int>();
        spt2.insert(10);
        // Merge the two trees together
        spt.mergeTree(&spt2);
        QCOMPARE(spt.ContainsValue(10), true);
    }

    //=========================================================================
    //      Arrangement: Tests
    //=========================================================================
};

QTEST_MAIN(tst_Geometry)
#include "tst_Geometry.moc"
