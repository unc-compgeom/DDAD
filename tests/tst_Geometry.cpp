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
        return SampleBundle(SampleSharedSegment(1, 1, 6, 2, true),
                            SampleSharedSegment(1, 4, 7, 5, true));
    }

    DDAD::Bundle SampleBundle(DDAD::SharedSegment seg1,
                              DDAD::SharedSegment seg2)
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

    DDAD::SharedBundle SampleSharedBundle(DDAD::SharedSegment seg)
    {
        DDAD::SharedBundle bdl = std::make_shared<DDAD::Bundle>();
        bdl->Insert(seg);
        return bdl;
    }

    DDAD::SharedBundle SampleSharedBundle(DDAD::SharedSegment seg1,
                                          DDAD::SharedSegment seg2)
    {
        return std::make_shared<DDAD::Bundle>(SampleBundle(seg1, seg2));
    }

    DDAD::Arrangement_2r SampleArrangement()
    {
    // Set up a sample arrangement for testing line intersections
    DDAD::Arrangement_2r sample_arrangement = DDAD::Arrangement_2r();
    DDAD::Point_2r p, q;
    p = DDAD::Point_2r(DDAD::rational(DDAD::integer(3), DDAD::integer(1)),
                       DDAD::rational(DDAD::integer(3), DDAD::integer(1)));
    q = DDAD::Point_2r(DDAD::rational(DDAD::integer(6), DDAD::integer(1)),
                       DDAD::rational(DDAD::integer(6), DDAD::integer(1)));
    sample_arrangement.AddSegment(p, q, true);
    p = DDAD::Point_2r(DDAD::rational(DDAD::integer(4), DDAD::integer(1)),
                       DDAD::rational(DDAD::integer(5), DDAD::integer(1)));
    q = DDAD::Point_2r(DDAD::rational(DDAD::integer(18), DDAD::integer(1)),
                       DDAD::rational(DDAD::integer(3), DDAD::integer(1)));
    sample_arrangement.AddSegment(p, q, false);
    return sample_arrangement;
    }

private slots:
    //=========================================================================
    //      Line: Tests
    //=========================================================================
    void SegmentComparators()
    {
        DDAD::Segment_2r_colored s1 = SampleSegment(5, 5, 10, 10, true);
        DDAD::Segment_2r_colored s2 = SampleSegment(2, 2, 10, 2, true);
        DDAD::Point_2r between_segments = DDAD::Point_2r(6, 4);
        DDAD::Point_2r above_s1 = DDAD::Point_2r(5, 10);
        DDAD::Point_2r below_s2 = DDAD::Point_2r(5, 0);
        DDAD::Point_2r on_s1 = DDAD::Point_2r(6, 6);
        DDAD::Point_2r on_s2 = DDAD::Point_2r(4, 2);
        QCOMPARE(DDAD::Predicate::AIsRightOfB(below_s2, s2.support()), true);
        QCOMPARE(DDAD::Predicate::AIsLeftOfB(above_s1, s1.support()), true);
        QCOMPARE(DDAD::Predicate::AIsRightOfB(below_s2, s1.support()), true);
        QCOMPARE(DDAD::Predicate::AIsLeftOfB(above_s1, s2.support()), true);
        QCOMPARE(DDAD::Predicate::AIsLeftOrAheadOfB(on_s1,
                                                    s1.support_ray()), true);
        QCOMPARE(DDAD::Predicate::AIsRightOrAheadOfB(on_s1,
                                                     s1.support_ray()), true);
        QCOMPARE(DDAD::Predicate::AIsRightOrAheadOfB(on_s2,
                                                     s2.support_ray()), true);
        QCOMPARE(DDAD::Predicate::AIsLeftOrAheadOfB(on_s2,
                                                    s2.support_ray()), true);
        QCOMPARE(DDAD::Predicate::AIsLeftOrAheadOfB(below_s2,
                                                    s2.support_ray()), false);
        QCOMPARE(DDAD::Predicate::AIsRightOrAheadOfB(above_s1,
                                                    s1.support_ray()), false);
        QCOMPARE(DDAD::Predicate::AIsLeftOfB(between_segments,
                                             s2.support()), true);
        QCOMPARE(DDAD::Predicate::AIsRightOfB(between_segments,
                                              s1.support()), true);
    }

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
        int current_count = spt.Size();
        spt.insert(49);
        QCOMPARE(spt.ContainsValue(49), true);
        QCOMPARE(spt.getRoot()->getElement(), 49);
        QCOMPARE(spt.Size(), current_count+1);

    }
    void SplayTreeRemove()
    {
        DDAD::SplayTree<int> spt = SampleSplayTree_int();
        // Remove 2 from the tree
        spt.remove(2);
        // Tree should now remove 2 from the tree
        QCOMPARE(spt.ContainsValue(2), false);
        spt.insert(30);
        spt.insert(20);
        int current_count = spt.Size();
        spt.remove(30);
        QCOMPARE(spt.Size(), current_count-1);
        current_count = spt.Size();
        spt.remove(100000);
        QCOMPARE(spt.Size(), current_count);
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
        spt.insert(30);
        spt.find(1);
        spt.find(49);
        QCOMPARE(spt.getRoot()->getElement(), 30);
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
        DDAD::SplayTree<int> spt = DDAD::SplayTree<int>();
        spt.insert(2);
        spt.insert(4);
        spt.insert(6);
        spt.insert(7);
        DDAD::SplayTree<int> spt2 = DDAD::SplayTree<int>();
        spt2.insert(10);
        spt2.insert(11);
        spt2.insert(12);
        // Merge the two trees together
        spt.mergeTree(&spt2);
        QCOMPARE(spt.getRoot()->getElement(), 7);
        QCOMPARE(spt.getRoot()->right->getElement(), 12);
        QCOMPARE(spt.ContainsValue(10), true);
        QCOMPARE(spt.Size(), 7);

        spt = DDAD::SplayTree<int>();
        spt2 = DDAD::SplayTree<int>();
        spt2.insert(2);
        spt2.insert(3);
        spt.mergeTree(&spt2);
        QVERIFY(!spt.isEmpty());
        QCOMPARE(spt.getRoot(), spt2.getRoot());

    }

    void SplayTreeSplit(){
        DDAD::SplayTree<int> spt = SampleSplayTree_int();
        DDAD::SplayTree<int> spt2 = DDAD::SplayTree<int>();
        //SplitTree should return the subtree >
        spt2 = spt.SplitTree(2);
        QCOMPARE(spt2.ContainsValue(2), false);
        QCOMPARE(spt2.ContainsValue(3), true);
        QCOMPARE(spt2.ContainsValue(4), true);
        QCOMPARE(spt.ContainsValue(2), true);
        QCOMPARE(spt.ContainsValue(1), true);
        QCOMPARE(spt.ContainsValue(3), false);
        DDAD::SplayTree<int> spt3 = DDAD::SplayTree<int>();
        spt3.insert(3);
        spt3.insert(4);
        spt2 = spt3.SplitTree(3);
        QCOMPARE(spt3.ContainsValue(3), true);
        QCOMPARE(spt3.ContainsValue(4), false);
        spt = DDAD::SplayTree<int>();
        spt2 = DDAD::SplayTree<int>();
        spt.insert(1);
        spt.insert(2);
        spt2 = spt.SplitTree(2);
        QCOMPARE(spt.ContainsValue(1), true);
        QCOMPARE(spt.ContainsValue(2), false);
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

        DDAD::Bundle bdl2 = DDAD::Bundle();
        bdl2.Insert(SampleSharedSegment(5, 5, 10, 10, true));
        bdl2.Insert(SampleSharedSegment(3, 3, 10, 3, true));
        test_vertex = SampleArrangementVertex(2, 2, true);
        QCOMPARE(bdl2.Contains(test_vertex), false);
        test_vertex = SampleArrangementVertex(6, 4, true);
        QCOMPARE(bdl2.Contains(test_vertex), true);
        test_vertex = SampleArrangementVertex(5, 5, true);
        QCOMPARE(bdl2.Contains(test_vertex), true);
        test_vertex = SampleArrangementVertex(10, 10, true);
        QCOMPARE(bdl2.Contains(test_vertex), true);
        test_vertex = SampleArrangementVertex(3, 3, true);
        QCOMPARE(bdl2.Contains(test_vertex), true);
        test_vertex = SampleArrangementVertex(10, 3, true);
        QCOMPARE(bdl2.Contains(test_vertex), true);
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
        sbdl3->Insert(SampleSharedSegment(0, 4, 20, 4, false));
        sbdl2->set_next_bundle(sbdl3);
        sbdl3->set_prev_bundle(sbdl2);
        QVERIFY(sbdl3->get_prev_bundle() == sbdl2);
        sbdl1->Merge(sbdl2);
        QVERIFY(sbdl3->get_prev_bundle() == sbdl1);
        QVERIFY(sbdl3->get_next_bundle() == nullptr);
        QVERIFY(sbdl1->get_next_bundle() == sbdl3);
        QVERIFY(sbdl1->get_prev_bundle() == nullptr);
        QVERIFY(sbdl1->get_root()->right == sbdl2->get_root());
        QCOMPARE(sbdl1->CountSegments(), 2);

        // Only two bundles in the list
        sbdl1 = SampleSharedBundle(SampleSharedSegment(2, 2, 8, 2, true));
        sbdl2 = SampleSharedBundle(SampleSharedSegment(2, 4, 8, 4, true));
        sbdl1->set_next_bundle(sbdl2);
        sbdl2->set_prev_bundle(sbdl1);
        sbdl1->Merge(sbdl2);
        QVERIFY(sbdl1->get_next_bundle() == nullptr);
        QVERIFY(sbdl1->get_prev_bundle() == nullptr);
        QVERIFY(sbdl1->get_root()->right == sbdl2->get_root());
        QCOMPARE(sbdl1->CountSegments(), 2);


    }

    void BundleSplit()
    {
        DDAD::SharedBundle sbdl1 = std::make_shared<DDAD::Bundle>();
        sbdl1->Insert(SampleSharedSegment(1, 1, 20, 1, true));
        sbdl1->Insert(SampleSharedSegment(1, 5, 5, 2, true));
        DDAD::SharedBundle sbdl2 =
                sbdl1->Split(SampleSharedSegment(1, 4, 5, 1, true));
        QCOMPARE(sbdl1->CountSegments(), 1);
        QCOMPARE(sbdl2->CountSegments(), 1);
    }

    void BundleCompare()
    {
        DDAD::SharedBundle bdlred =
                SampleSharedBundle(SampleSharedSegment(1, 1, 5, 1, true),
                                   SampleSharedSegment(1, 3, 12, 3, true));
        DDAD::SharedBundle bdlred2 =
                SampleSharedBundle(SampleSharedSegment(1, 4, 5, 4, true),
                                   SampleSharedSegment(1, 6, 3, 7, true));
        QCOMPARE(*bdlred == *bdlred2, false);
        QCOMPARE(*bdlred > *bdlred2, false);
        QCOMPARE(*bdlred < *bdlred2, true);
    }

    void BundleTreeInsert()
    {
        DDAD::BundleTree bdt = DDAD::BundleTree();
        DDAD::SharedBundle bdlred =
                SampleSharedBundle(SampleSharedSegment(1, 1, 7, 2, true),
                             SampleSharedSegment(1, 2, 8, 3, true));
        DDAD::SharedBundle bdlred2 =
                SampleSharedBundle(SampleSharedSegment(1, 4, 7, 4, true),
                                   SampleSharedSegment(1, 7, 3, 7, true));
        DDAD::SharedBundle bdlblue =
                SampleSharedBundle(SampleSharedSegment(1, 5, 10, 5, false),
                                   SampleSharedSegment(1, 6, 6, 6, false));

        //insert a red bundle
        bdt.InsertBundle(bdlred);
        //try to insert a blue bundle
        bdt.InsertBundle(bdlblue);
        QCOMPARE(bdt.get_root(), bdlred);
        //size of bdt should be 1, since the blue bundle should not insert
        QCOMPARE(bdt.Size(), 1);
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
        DDAD::SharedBundle bdlred =
                SampleSharedBundle(SampleSharedSegment(1, 1, 7, 2, true),
                             SampleSharedSegment(1, 2, 8, 3, true));
        DDAD::SharedBundle bdlred2 =
                SampleSharedBundle(SampleSharedSegment(1, 4, 7, 4, true),
                                   SampleSharedSegment(1, 7, 3, 7, true));

        //insert a red bundle
        bdt.InsertBundle(bdlred);
        //insert a second red bundle
        bdt.InsertBundle(bdlred2);
        //remove first red bundle
        bdt.RemoveBundle(bdlred);
        //second red bundle should be at the root, size should be 1
        QCOMPARE(bdt.get_root(), bdlred2);
        QCOMPARE(bdt.Size(), 1);
    }

    void BundleTreeFind()
    {
        DDAD::BundleTree bdt = DDAD::BundleTree();
        DDAD::SharedBundle bdlred = SampleSharedBundle();
        DDAD::SharedBundle bdlred2 =
                SampleSharedBundle(SampleSharedSegment(0, 4, 5, 4, true),
                                   SampleSharedSegment(1, 6, 3, 7, true));
        bdt.InsertBundle(bdlred);
        bdt.InsertBundle(bdlred2);

        DDAD::ArrangementVertex_2r vertex = SampleArrangementVertex(0,4,true);
        bdt.Find(vertex);
        QCOMPARE(bdt.get_root(),bdlred2);

    }

    void BundleTreeLocateVertex()
    {
        DDAD::BundleTree bdt = DDAD::BundleTree();
        DDAD::SharedBundle bdlred1 =
                SampleSharedBundle(SampleSharedSegment(2, 2, 8, 2, true),
                                   SampleSharedSegment(3, 3, 8, 3, true));
        DDAD::SharedBundle bdlred2 =
                SampleSharedBundle(SampleSharedSegment(0, 4, 10, 5, true),
                                   SampleSharedSegment(1 ,6 ,12 ,7 ,true));
        DDAD::SharedBundle above, below;
        bdt.InsertBundle(bdlred1);
        bdt.InsertBundle(bdlred2);
        DDAD::ArrangementVertex_2r between_bundles =
                SampleArrangementVertex(6, 4, true);
        DDAD::ArrangementVertex_2r below_both =
                SampleArrangementVertex(4, 1, true);
        DDAD::ArrangementVertex_2r above_both =
                SampleArrangementVertex(4, 15, true);

        bdt.LocateVertex(between_bundles, above, below);
        QCOMPARE(above, bdlred2);
        QCOMPARE(below, bdlred1);

        bdt.LocateVertex(below_both, above, below);
        QCOMPARE(above, bdlred1);
        QVERIFY(below == nullptr);

        bdt.LocateVertex(above_both, above, below);
        QCOMPARE(below, bdlred2);
        QVERIFY(above == nullptr);

        bdt = DDAD::BundleTree();
        bdlred1 =
                SampleSharedBundle(SampleSharedSegment(4, 5, 6, 6, true));
        DDAD::SharedBundle bdlblue =
                SampleSharedBundle(SampleSharedSegment(3, 8, 10, 1, false));
        bdt.InsertBundle(bdlred1);
        bdt.InsertBundle(bdlblue);
        DDAD::ArrangementVertex_2r endpt1 = SampleArrangementVertex(6, 6, true);
        bdt.LocateVertex(endpt1, above, below);
        QCOMPARE(below, bdlred1);
        QCOMPARE(above, bdlred1);

        // Pre-test the case for the BundleList associated method
        bdt = DDAD::BundleTree();
        DDAD::SharedBundle red =
                SampleSharedBundle(SampleSharedSegment(3, 2, 12, 2, true));
        DDAD::SharedBundle red2 =
                SampleSharedBundle(SampleSharedSegment(3, 8, 12, 8, true));
        DDAD::SharedBundle red3 =
                SampleSharedBundle(SampleSharedSegment(3, 12, 12, 12, true));
        DDAD::ArrangementVertex_2r on_r1 =
                SampleArrangementVertex(3, 2, true);
        DDAD::ArrangementVertex_2r on_r2 =
                SampleArrangementVertex(3, 8, true);
        DDAD::ArrangementVertex_2r between_r1_r2 =
                SampleArrangementVertex(9, 6, true);
        bdt.InsertBundle(red);
        bdt.InsertBundle(red2);
        bdt.InsertBundle(red3);
        bdt.LocateVertex(on_r1, above, below);
        QVERIFY(above == red);
        QVERIFY(below == red);

        bdt.LocateVertex(on_r2, above, below);
        QVERIFY(above == red2);
        QVERIFY(below == red2);

        bdt.LocateVertex(between_r1_r2, above, below);
        QVERIFY(above == red2);
        QVERIFY(below == red);
    }

    void BundleTreeSplitAtVertex()
    {
        DDAD::BundleTree bdt = DDAD::BundleTree();
        DDAD::SharedBundle bdlred = SampleSharedBundle();
        DDAD::SharedBundle bdlred2 =
                SampleSharedBundle(SampleSharedSegment(0, 4, 5, 4, true),
                                   SampleSharedSegment(1, 6, 3, 7, true));
        bdt.InsertBundle(bdlred);
        bdt.InsertBundle(bdlred2);

        DDAD::ArrangementVertex_2r vertex = SampleArrangementVertex(2,5,true);

        bdt.SplitBundleAtVertex(vertex);
        QCOMPARE(bdt.Size(), 3);
    }

    void BundleListInsert()
    {
        DDAD::BundleList bdl = DDAD::BundleList();
        DDAD::SharedBundle bundle = SampleSharedBundle();
        DDAD::SharedBundle bundle2 =
                SampleSharedBundle(SampleSharedSegment(0, 1, 1, 2, false),
                                   SampleSharedSegment(0, 2, 3, 3, false));
        QVERIFY(bdl.get_bottom() == nullptr);
        QVERIFY(bdl.get_top() == nullptr);
        bdl.InsertBundle(bundle,nullptr);
        QCOMPARE(bdl.get_bottom(), bundle);
        QCOMPARE(bdl.get_top(), bundle);
        bdl.InsertBundle(bundle2,bundle);
        QCOMPARE(bdl.get_bottom(), bundle);
        QCOMPARE(bdl.get_top(), bundle2);
    }

    void BundleListRemove()
    {
        DDAD::BundleList bdl = DDAD::BundleList();
        DDAD::SharedBundle bundle = SampleSharedBundle();
        DDAD::SharedBundle bundle2 =
                SampleSharedBundle(SampleSharedSegment(0, 1, 1, 2, false),
                                   SampleSharedSegment(0, 2, 3, 3, false));
        bdl.InsertBundle(bundle,nullptr);
        bdl.InsertBundle(bundle2,bundle);

        bdl.RemoveBundle(bundle);
        QCOMPARE(bdl.get_bottom(), bundle2);
        QCOMPARE(bdl.get_top(), bundle2);

        // Test on a single-bundle list
        DDAD::BundleList bdl2 = DDAD::BundleList();
        DDAD::SharedBundle bundle1 = SampleSharedBundle();
        bdl2.InsertBundle(bundle1, nullptr);
        QCOMPARE(bdl2.get_top(), bundle1);
        bdl2.RemoveBundle(bundle1);
        QVERIFY(bdl2.get_top() == nullptr);

        // Swap colors
        bdl = DDAD::BundleList();
        bundle = SampleSharedBundle(SampleSharedSegment(1, 1, 2, 2, false),
                                    SampleSharedSegment(1, 5, 3, 2, false));
        bundle2 =
                SampleSharedBundle(SampleSharedSegment(0, 1, 1, 2, true),
                                   SampleSharedSegment(0, 2, 3, 3, true));
        bdl.InsertBundle(bundle, nullptr);
        bdl.InsertBundle(bundle2, bundle);

        bdl.RemoveBundle(bundle);
        QCOMPARE(bdl.get_bottom(), bundle2);
        QCOMPARE(bdl.get_top(), bundle2);
        QVERIFY(bundle2->get_next_bundle() == nullptr);
        QVERIFY(bundle2->get_prev_bundle() == nullptr);

    }

    void BundleListLocateVertex()
    {
        DDAD::BundleList bdl = DDAD::BundleList();
        DDAD::BundleTree bdt = DDAD::BundleTree();
        DDAD::SharedBundle red =
                SampleSharedBundle(SampleSharedSegment(3, 2, 12, 2, true));
        DDAD::SharedBundle red2 =
                SampleSharedBundle(SampleSharedSegment(3, 8, 12, 8, true));
        DDAD::SharedBundle red3 =
                SampleSharedBundle(SampleSharedSegment(3, 12, 12, 12, true));
        DDAD::SharedBundle blue =
                SampleSharedBundle(SampleSharedSegment(3, 9, 11, 11, false));
        DDAD::SharedBundle blue2 =
                SampleSharedBundle(SampleSharedSegment(4, 7, 12, 1, false));
        DDAD::SharedBundle above, below;
        DDAD::ArrangementVertex_2r on_r1 =
                SampleArrangementVertex(3, 2, true);
        DDAD::ArrangementVertex_2r on_b2 =
                SampleArrangementVertex(4, 7, false);
        DDAD::ArrangementVertex_2r between_b2_r2 =
                SampleArrangementVertex(9, 6, true);
        bdl.InsertBundle(red, nullptr);
        bdl.InsertBundle(blue2, red);
        bdl.InsertBundle(red2, blue2);
        bdl.InsertBundle(blue, red2);
        bdl.InsertBundle(red3, blue);
        bdt.InsertBundle(red);
        bdt.InsertBundle(red2);
        bdt.InsertBundle(red3);
        bdt.LocateVertex(on_r1, above, below);
        bdl.LocateVertex(on_r1, above, below, bdt);
        QVERIFY(below == red);
        QVERIFY(above == blue2);

        bdt.LocateVertex(on_b2, above, below);
        bdl.LocateVertex(on_b2, above, below, bdt);
        QVERIFY(above == red2);
        QVERIFY(below == red);

        bdt.LocateVertex(between_b2_r2, above, below);
        bdl.LocateVertex(between_b2_r2, above, below, bdt);
        QVERIFY(above == blue);
        QVERIFY(below == red);


    }

    void BundleListSortPortion()
    {
        DDAD::BundleList bdl = DDAD::BundleList();
        DDAD::SharedBundle red =
                SampleSharedBundle(SampleSharedSegment(3, 3, 6, 6, true));
        DDAD::SharedBundle red2 =
                SampleSharedBundle(SampleSharedSegment(7, 2, 8, 4, true));
        DDAD::SharedBundle blue =
                SampleSharedBundle(SampleSharedSegment(3, 8, 9, 2, false));
        int numIntersections = 0;

        bdl.InsertBundle(blue, nullptr);
        bdl.InsertBundle(red, nullptr);

        QVERIFY(red->get_prev_bundle() == nullptr);
        QVERIFY(blue->get_next_bundle() == nullptr);
        QVERIFY(red->get_next_bundle() == blue);
        QVERIFY(blue->get_prev_bundle() == red);
        QVERIFY(bdl.get_bottom() == red);
        QVERIFY(bdl.get_top() == blue);
        numIntersections +=
                bdl.SortPortion(red, blue,
                                SampleArrangementVertex(6, 6, true));

        QVERIFY(red->get_next_bundle() == nullptr);
        QVERIFY(red->get_prev_bundle() == blue);
        QVERIFY(blue->get_prev_bundle() == nullptr);
        QVERIFY(blue->get_next_bundle() == red);
        QVERIFY(bdl.get_bottom() == blue);
        QVERIFY(bdl.get_top() == red);
        QCOMPARE(numIntersections, 1);

        bdl.RemoveBundle(red);
        bdl.InsertBundle(red2, nullptr);
        numIntersections +=
                bdl.SortPortion(red2, blue,
                                SampleArrangementVertex(8, 4, true));
        QVERIFY(red2->get_next_bundle() == nullptr);
        QVERIFY(blue ->get_prev_bundle() == nullptr);
        QCOMPARE(numIntersections, 2);

        // Swap red/blue
        bdl = DDAD::BundleList();
        blue =
                SampleSharedBundle(SampleSharedSegment(3, 3, 6, 6, false));
        DDAD::SharedBundle blue2 =
                SampleSharedBundle(SampleSharedSegment(7, 2, 8, 4, false));
        red =
                SampleSharedBundle(SampleSharedSegment(3, 8, 9, 2, true));
        numIntersections = 0;

        bdl.InsertBundle(red, nullptr);
        bdl.InsertBundle(blue, nullptr);

        QVERIFY(blue->get_prev_bundle() == nullptr);
        QVERIFY(red->get_next_bundle() == nullptr);
        QVERIFY(blue->get_next_bundle() == red);
        QVERIFY(red->get_prev_bundle() == blue);
        QVERIFY(bdl.get_bottom() == blue);
        QVERIFY(bdl.get_top() == red);
        numIntersections +=
                bdl.SortPortion(blue, red,
                                SampleArrangementVertex(6, 6, false));

        QVERIFY(blue->get_next_bundle() == nullptr);
        QVERIFY(blue->get_prev_bundle() == red);
        QVERIFY(red->get_next_bundle() == blue);
        QVERIFY(red->get_prev_bundle() == nullptr);
        QVERIFY(bdl.get_bottom() == red);
        QVERIFY(bdl.get_top() == blue);
        QCOMPARE(numIntersections, 1);

        bdl.RemoveBundle(blue);
        bdl.InsertBundle(blue2, nullptr);
        numIntersections +=
                bdl.SortPortion(blue2, red,
                                SampleArrangementVertex(8, 4, false));
        QVERIFY(blue2->get_next_bundle() == nullptr);
        QVERIFY(red->get_prev_bundle() == nullptr);
        QCOMPARE(numIntersections, 2);

    }

    void BundleListSwapAdjacentBundles()
    {
        // Try with only two bundles
        DDAD::BundleList bdl = DDAD::BundleList();
        DDAD::SharedBundle red =
                SampleSharedBundle(SampleSharedSegment(3, 3, 6, 6, true));
        DDAD::SharedBundle blue =
                SampleSharedBundle(SampleSharedSegment(3, 8, 9, 2, false));
        bdl.InsertBundle(blue, nullptr);
        bdl.InsertBundle(red, nullptr);
        bdl.SwapAdjacentBundles(red, blue);
        QVERIFY(red->get_next_bundle() == nullptr);
        QVERIFY(red->get_prev_bundle() == blue);
        QVERIFY(blue->get_prev_bundle() == nullptr);
        QVERIFY(blue->get_next_bundle() == red);
        QVERIFY(bdl.get_bottom() == blue);
        QVERIFY(bdl.get_top() == red);

        // Try a longer list of bundles
        bdl = DDAD::BundleList();
        red = SampleSharedBundle(SampleSharedSegment(3, 3, 6, 6, true));
        DDAD::SharedBundle red2 =
                SampleSharedBundle(SampleSharedSegment(3, 9, 7, 9, true));
        blue = SampleSharedBundle(SampleSharedSegment(3, 8, 9, 2, false));
        DDAD::SharedBundle blue2 =
                SampleSharedBundle(SampleSharedSegment(3, 3, 5, 4, false));
        bdl.InsertBundle(blue2, nullptr);
        bdl.InsertBundle(red, blue2);
        bdl.InsertBundle(blue, red);
        bdl.InsertBundle(red2, blue);
        QVERIFY(bdl.get_bottom() == blue2);
        QVERIFY(bdl.get_top() == red2);
        bdl.SwapAdjacentBundles(red, blue);
        QVERIFY(bdl.get_bottom() == blue2);
        QVERIFY(bdl.get_top() == red2);
        QVERIFY(bdl.get_bottom()->get_next_bundle() == blue);
        QVERIFY(bdl.get_top()->get_prev_bundle() == red);
    }

    void CountIntersections()
    {
        // Simple arrangements
        DDAD::Arrangement_2r sample_arrangement = DDAD::Arrangement_2r();
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(4, 5), DDAD::Point_2r(6, 6), true);
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(3, 8), DDAD::Point_2r(10, 1), false);
        int intersections = DDAD::CountIntersections(sample_arrangement);
        // Should just be two intersecting lines
        QCOMPARE(intersections, 1);

        // Add a second red line that also intersects with the blue line
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(7, 2), DDAD::Point_2r(8, 4), true);
        intersections = DDAD::CountIntersections(sample_arrangement);
        QCOMPARE(intersections, 2);


        // Add another blue line that does not intersect any existing lines
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(30, 30), DDAD::Point_2r(40, 40), true);
        intersections = DDAD::CountIntersections(sample_arrangement);
        QCOMPARE(intersections, 2);


        // Same arrangement, but with coloring order swapped
        sample_arrangement = DDAD::Arrangement_2r();
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(4, 5), DDAD::Point_2r(6, 6), false);
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(3, 8), DDAD::Point_2r(10, 1), true);
        intersections = DDAD::CountIntersections(sample_arrangement);
        // Should just be two intersecting lines
        QCOMPARE(intersections, 1);

        // Add a second red line that also intersects with the blue line
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(7, 2), DDAD::Point_2r(8, 4), false);
        intersections = DDAD::CountIntersections(sample_arrangement);
        QCOMPARE(intersections, 2);


        // Add another blue line that does not intersect any existing lines
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(30, 30), DDAD::Point_2r(40, 40), false);
        intersections = DDAD::CountIntersections(sample_arrangement);
        QCOMPARE(intersections, 2);

        // Only red segments
        sample_arrangement = DDAD::Arrangement_2r();
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(2, 10), DDAD::Point_2r(10, 10), true);
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(2, 5), DDAD::Point_2r(10, 5), true);
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(2, 2), DDAD::Point_2r(10, 2), true);
        intersections = DDAD::CountIntersections(sample_arrangement);
        QCOMPARE(intersections, 0);

        // Grid arrangement
        sample_arrangement = DDAD::Arrangement_2r();
        for(int ii = 2; ii < 22; ii++)
        {
            sample_arrangement.AddSegment(
                        DDAD::Point_2r(ii, 1), DDAD::Point_2r(ii, 24), true);
            sample_arrangement.AddSegment(
                        DDAD::Point_2r(1, ii), DDAD::Point_2r(24, ii), false);
        }
        intersections = DDAD::CountIntersections(sample_arrangement);
        QCOMPARE(intersections, 400);

    }
};


QTEST_MAIN(tst_Geometry)
#include "tst_Geometry.moc"
