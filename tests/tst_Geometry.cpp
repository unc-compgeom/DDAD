#include <QtTest>
#include "../geometry/common.h"
#include "../geometry/arithmetic.h"
#include "../geometry/datastructures.h"
#include "../geometry/arrangement.h"
#include "../geometry/line.h"


class tst_Geometry : public QObject {
    Q_OBJECT
public:
    //=========================================================================
    //      Line: Factories
    //=========================================================================
    DDAD::Segment_2r_colored SampleSegment()
    {
        return SampleSegment(0, 0, 1, 1, true);
    }
    DDAD::Segment_2r_colored SampleSegment(int px, int py,
                                           int qx, int qy, bool is_red)
    {
        DDAD::Point_2r p = DDAD::Point_2r(px, py);
        DDAD::Point_2r q = DDAD::Point_2r(qx, qy);
        DDAD::SharedPoint_2r p_sptr = std::make_shared<DDAD::Point_2r>(p);
        DDAD::SharedPoint_2r q_sptr = std::make_shared<DDAD::Point_2r>(q);
        DDAD::Segment_2r_colored segment =
                DDAD::Segment_2r_colored(p_sptr, q_sptr, is_red);
        return segment;
    }

    DDAD::SharedSegment SampleSharedSegment(int px, int py, int qx,
                                            int qy, bool is_red)
    {
        DDAD::Segment_2r_colored segment =
                SampleSegment(px, py, qx, qy, is_red);
        DDAD::SharedSegment shared_seg =
                std::make_shared<DDAD::Segment_2r_colored>(segment);
        return shared_seg;
    }

    //=========================================================================
    //      Datastructures: Factories
    //=========================================================================
    DDAD::BinaryNode<int> SampleBinaryNode_int()
    {
        DDAD::BinaryNode<int> bnd = DDAD::BinaryNode<int>();
        bnd.left = nullptr;
        bnd.right = nullptr;
        bnd.element = 1;
        return bnd;
    }

    DDAD::SplayTree<int> SampleSplayTree_int()
    {
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
    DDAD::ArrangementVertex_2r SampleArrangementVertex(int x, int y,
                                                       bool color)
    {
        DDAD::SharedSegment tmp_segment =
                SampleSharedSegment(x, y, x+1, y+1, color);
        DDAD::ArrangementVertex_2r vert =
                DDAD::ArrangementVertex_2r(tmp_segment->p_sptr(),
                                           tmp_segment->q_sptr(),
                                           tmp_segment->get_color());
        return vert;
    }

    DDAD::Bundle SampleBundle()
    {
        return SampleBundle(SampleSharedSegment(0,0,1,1,true),SampleSharedSegment(0,5,3,2, true));
    }

    DDAD::Bundle SampleBundle(DDAD::SharedSegment seg1, DDAD::SharedSegment seg2)
    {
        DDAD::Bundle bdl = DDAD::Bundle();
        bdl.Insert(seg1);
        bdl.Insert(seg2);
        return bdl;
    }

    DDAD::SharedBundle SampleSharedBundle()
    {
        return std::make_shared<DDAD::Bundle>(SampleBundle());
    }

    DDAD::SharedBundle SampleSharedBundle(DDAD::SharedSegment seg1, DDAD::SharedSegment seg2)
    {
        return std::make_shared<DDAD::Bundle>(SampleBundle(seg1, seg2));
    }

private slots:
    //=========================================================================
    //      Datastructures: Tests
    //=========================================================================
    void BinaryNodeConstructors()
    {
        DDAD::BinaryNode<int> tmpNode = SampleBinaryNode_int();
        QCOMPARE(tmpNode.getElement(), 1);

        // Try the copy constructor as well
        DDAD::BinaryNode<int> node2 =
                DDAD::BinaryNode<int>(2, &tmpNode, nullptr);
        QCOMPARE(node2.left->getElement(), 1);
    }


    void SplayTreeConstructors()
    {
        DDAD::SplayTree<int> spt = DDAD::SplayTree<int>();
        QVERIFY(spt.getRoot()==nullptr);

        spt = SampleSplayTree_int();
        DDAD::SplayTree<int> spt2 = spt;
        QCOMPARE(spt2.getRoot()->getElement(), spt.getRoot()->getElement());
    }

    void SplayTreeInserts()
    {
        DDAD::SplayTree<int> spt = SampleSplayTree_int();
        // Inserting an element should splay to the top
        spt.insert(20);
        QCOMPARE(spt.ContainsValue(20), true);
        QCOMPARE(spt.getRoot()->getElement(), 20);

    }
    void SplayTreeRemove()
    {
        DDAD::SplayTree<int> spt = SampleSplayTree_int();
        // Remove 2 from the tree
        spt.remove(2);
        // Tree should now remove 2 from the tree
        QCOMPARE(spt.ContainsValue(2), false);
    }
    void SplayTreeFind()
    {
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

    void SplayTreeMinMax()
    {
        DDAD::SplayTree<int> spt = SampleSplayTree_int();
        //findMax should splay the maximum element to the root of the tree
        spt.findMax();
        QCOMPARE(spt.getRoot()->getElement(), 4);
        //findMin should splay the minimum element to the root of the tree
        spt.findMin();
        QCOMPARE(spt.getRoot()->getElement(), 0);
    }

    void SplayTreeMerge()
    {
        DDAD::SplayTree<int> spt = SampleSplayTree_int();
        DDAD::SplayTree<int> spt2 = DDAD::SplayTree<int>();
        spt2.insert(10);
        // Merge the two trees together
        spt.mergeTree(&spt2);
        QCOMPARE(spt.ContainsValue(10), true);
    }

    void SplayTreeSplit(){
        DDAD::SplayTree<int> spt = SampleSplayTree_int();
        DDAD::SplayTree<int> spt2 = DDAD::SplayTree<int>();
        //splitTree should return the subtree >
        spt2 = spt.splitTree(2);
        QCOMPARE(spt2.ContainsValue(2), false);
        QCOMPARE(spt2.ContainsValue(3), true);
        QCOMPARE(spt2.ContainsValue(4), true);
        QCOMPARE(spt.ContainsValue(2), true);
        QCOMPARE(spt.ContainsValue(1), true);
        QCOMPARE(spt.ContainsValue(3), false);
    }



    //=========================================================================
    //      Arrangement: Tests
    //=========================================================================

    void BundleConstructor()
    {
        DDAD::Bundle bdl = DDAD::Bundle();
        QVERIFY(bdl.get_root()==nullptr);
    }

    void BundleSetNextPrev()
    {
        DDAD::SharedBundle sbdl1 =
                std::make_shared<DDAD::Bundle>(SampleBundle());
        DDAD::SharedBundle sbdl2 =
                std::make_shared<DDAD::Bundle>();
        sbdl2->Insert(SampleSharedSegment(2, 2, 3, 3, true));
        sbdl2->Insert(SampleSharedSegment(2, 2, 4, 4, true));
        sbdl1->set_next_bundle(sbdl2);
        sbdl2->set_prev_bundle(sbdl1);
        QVERIFY(sbdl1->get_next_bundle() == sbdl2);
        DDAD::SharedBundle sbdl3 =
                std::make_shared<DDAD::Bundle>();
        sbdl3->Insert(SampleSharedSegment(0, 10, 3, 10, true));
        sbdl2->set_next_bundle(sbdl3);
        sbdl3->set_prev_bundle(sbdl2);
        QVERIFY(sbdl1->get_next_bundle()->get_next_bundle()==sbdl3);
        QVERIFY(sbdl3->get_prev_bundle()->get_prev_bundle()==sbdl1);
    }

    void BundleInsert()
    {
        DDAD::Bundle bdl = SampleBundle();
        int current_segments = bdl.CountSegments();
        // Non-parallel segments
        bdl.Insert(SampleSharedSegment(0, 1, 11, 11, true));
        QCOMPARE(bdl.CountSegments(), current_segments+1);
        bdl = SampleBundle();
        // Parallel segments
        bdl.Insert(SampleSharedSegment(0, 2, 5, 2, true));
        current_segments = bdl.CountSegments();
        bdl.Insert(SampleSharedSegment(0, 4, 5, 4, true));
        QCOMPARE(bdl.CountSegments(), current_segments+1);
        // Contains a point between two inserted segments
        bdl = DDAD::Bundle();
        bdl.Insert(SampleSharedSegment(0, 2, 5, 2, true));
        bdl.Insert(SampleSharedSegment(0, 5, 5, 5, true));
        QCOMPARE(bdl.Contains(SampleArrangementVertex(3, 3, true)), true);
    }

    void BundleRemove()
    {
        DDAD::Bundle bdl = SampleBundle();
        DDAD::SharedSegment to_be_removed =
                SampleSharedSegment(100, 100, 120, 120, true);
        bdl.Insert(to_be_removed);
        // Try to remove a segment
        int current_count = bdl.CountSegments();
        bdl.Remove(to_be_removed);
        QCOMPARE(bdl.CountSegments(), current_count - 1);
    }

    void BundleContains()
    {
        DDAD::Bundle bdl = SampleBundle();
        bdl.Insert(SampleSharedSegment(-2, 2, 2, 2, true));
        DDAD::ArrangementVertex_2r test_vertex =
                SampleArrangementVertex(0, 1, true);
        QCOMPARE(bdl.Contains(test_vertex), true);

    }



    void BundleRelativePosition()
    {
        DDAD::Bundle bdl = SampleBundle();
        // Contains (0,0)->(1,1) and (0,5)->(10,5)
        DDAD::ArrangementVertex_2r test_vertex =
                SampleArrangementVertex(2, 10, true);
        DDAD::RelativePosition rel_position =
                bdl.SetRelativePosition(test_vertex);
        QCOMPARE(rel_position, DDAD::BELOW);
    }

    void BundleCountSegments()
    {
        DDAD::Bundle bdl = SampleBundle();
        QCOMPARE(bdl.CountSegments(), 2);
        DDAD::Bundle bdl2 = DDAD::Bundle();
        QCOMPARE(bdl2.CountSegments(), 0);
    }


    void BundleMerge()
    {
        DDAD::SharedBundle sbdl1 = std::make_shared<DDAD::Bundle>();
        sbdl1->Insert(SampleSharedSegment(0, 0, 20, 0, true));
        DDAD::SharedBundle sbdl2 = std::make_shared<DDAD::Bundle>();
        sbdl2->Insert(SampleSharedSegment(1, 1, 5, 2, true));
        sbdl1->set_next_bundle(sbdl2);
        sbdl2->set_prev_bundle(sbdl1);
        DDAD::SharedBundle sbdl3 = std::make_shared<DDAD::Bundle>();
        sbdl3->Insert(SampleSharedSegment(0, 4, 20, 4, true));
        sbdl2->set_next_bundle(sbdl3);
        sbdl3->set_prev_bundle(sbdl2);
        QVERIFY(sbdl3->get_prev_bundle() == sbdl2);
        sbdl1->Merge(sbdl2);
        QVERIFY(sbdl3->get_prev_bundle() == sbdl1);
        QCOMPARE(sbdl1->CountSegments(), 2);
    }

    void BundleSplit()
    {
        DDAD::SharedBundle sbdl1 = std::make_shared<DDAD::Bundle>();
        sbdl1->Insert(SampleSharedSegment(0, 0, 20, 0, true));
        sbdl1->Insert(SampleSharedSegment(1, 5, 5, 2, true));
        DDAD::SharedBundle sbdl2 =
                sbdl1->Split(SampleSharedSegment(1, 4, 5, 1, true));
        QCOMPARE(sbdl1->CountSegments(), 1);
        QCOMPARE(sbdl2->CountSegments(), 1);
    }

    void BundleCompare()
    {
        DDAD::SharedBundle bdlred = SampleSharedBundle();
        DDAD::SharedBundle bdlred2 = SampleSharedBundle(SampleSharedSegment(0,4,5,4,true),
                                                        SampleSharedSegment(1,6,3,7,true));
        QCOMPARE(bdlred == bdlred2, false);
        QCOMPARE(bdlred > bdlred2, false);
        QCOMPARE(bdlred < bdlred2, true);
    }

    void BundleTreeInsert()
    {
        DDAD::BundleTree bdt = DDAD::BundleTree();
        DDAD::SharedBundle bdlred = SampleSharedBundle();
        DDAD::SharedBundle bdlred2 = SampleSharedBundle(SampleSharedSegment(0,4,5,4,true),
                                                        SampleSharedSegment(1,6,3,7,true));
        DDAD::SharedBundle bdlblue = SampleSharedBundle(SampleSharedSegment(0,5,10,5,false),
                                                        SampleSharedSegment(0,6,6,6,false));

        //insert a red bundle
        bdt.InsertBundle(bdlred);
        //try to insert a blue bundle
        bdt.InsertBundle(bdlblue);
        QCOMPARE(bdt.get_root(), bdlred);
        //size of bdt should be 1, since the blue bundle should not insert
        QCOMPARE(bdt.Size(),1);
        //insert another red bundle
        bdt.InsertBundle(bdlred2);
        //size of bdt should be 2, now that we've added 2 red bundles
        QCOMPARE(bdt.Size(), 2);
        bdt.InsertBundle(bdlred2);
        //shouldn't be able to insert duplicates
        QCOMPARE(bdt.Size(), 2);
    }

    void BundleTreeRemove()
    {
        DDAD::BundleTree bdt = DDAD::BundleTree();
        DDAD::SharedBundle bdlred = SampleSharedBundle();
        DDAD::SharedBundle bdlred2 = SampleSharedBundle(SampleSharedSegment(0,4,5,4,true),
                                                        SampleSharedSegment(1,6,3,7,true));

        //insert a red bundle
        bdt.InsertBundle(bdlred);
        //insert a second red bundle
        bdt.InsertBundle(bdlred2);
        //remove first red bundle
        bdt.RemoveBundle(bdlred);
        //second red bundle should be at the root, size should be 1
        QCOMPARE(bdt.get_root(), bdlred2);
        QCOMPARE(bdt.Size(),1);
    }

    void BundleTreeFind()
    {
        DDAD::BundleTree bdt = DDAD::BundleTree();
        DDAD::SharedBundle bdlred = SampleSharedBundle();
        DDAD::SharedBundle bdlred2 = SampleSharedBundle(SampleSharedSegment(0,4,5,4,true),
                                                        SampleSharedSegment(1,6,3,7,true));
        bdt.InsertBundle(bdlred);
        bdt.InsertBundle(bdlred2);

        DDAD::ArrangementVertex_2r vertex = SampleArrangementVertex(0,4,true);
        bdt.Find(vertex);
        QCOMPARE(bdt.get_root(),bdlred2);

    }

    void BundleTreeLocateVertex()
    {
        DDAD::BundleTree bdt = DDAD::BundleTree();
        DDAD::SharedBundle bdlred = SampleSharedBundle();
        DDAD::SharedBundle bdlred2 = SampleSharedBundle(SampleSharedSegment(0,4,5,4,true),
                                                        SampleSharedSegment(1,6,3,7,true));
        DDAD::SharedBundle above, below;

        bdt.InsertBundle(bdlred);
        bdt.InsertBundle(bdlred2);

        bdt.LocateVertex(SampleArrangementVertex(0,4,true),above,below);
        QVERIFY(above == nullptr);
        QCOMPARE(below,bdlred2);

    }




};


QTEST_MAIN(tst_Geometry)
#include "tst_Geometry.moc"
