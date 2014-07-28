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
//    DDAD::BinaryNode<int> SampleBinaryNode_int()
//    {
//        DDAD::BinaryNode<int> bnd = DDAD::BinaryNode<int>();
//        bnd.left = nullptr;
//        bnd.right = nullptr;
//        bnd.element = 1;
//        return bnd;
//    }

//    DDAD::SplayTree<int> SampleSplayTree_int()
//    {
//        DDAD::SplayTree<int> spt = DDAD::SplayTree<int>();
//        int toinsert[] = {1, 2, 3, 4, 0};
//        for(int ii = 0; ii < 5; ii++){
//            spt.insert(toinsert[ii]);
//        }
//        return spt;
//    }

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
                                           tmp_segment->get_color(),
                                           tmp_segment);
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
//    void BinaryNodeConstructors()
//    {
//        DDAD::BinaryNode<int> tmpNode = SampleBinaryNode_int();
//        QCOMPARE(tmpNode.getElement(), 1);

//        // Try the copy constructor as well
//        DDAD::BinaryNode<int> node2 =
//                DDAD::BinaryNode<int>(2, &tmpNode, nullptr);
//        QCOMPARE(node2.left->getElement(), 1);
//    }

//    void SplayTreeConstructors()
//    {
//        DDAD::SplayTree<int> spt = DDAD::SplayTree<int>();
//        QVERIFY(spt.getRoot()==nullptr);

//        spt = SampleSplayTree_int();
//        DDAD::SplayTree<int> spt2 = spt;
//        QCOMPARE(spt2.getRoot()->getElement(), spt.getRoot()->getElement());
//    }

//    void SplayTreeInserts()
//    {
//        DDAD::SplayTree<int> spt = SampleSplayTree_int();
//        // Inserting an element should splay to the top
//        spt.insert(20);
//        QCOMPARE(spt.ContainsValue(20), true);
//        QCOMPARE(spt.getRoot()->getElement(), 20);
//        int current_count = spt.Size();
//        spt.insert(49);
//        QCOMPARE(spt.ContainsValue(49), true);
//        QCOMPARE(spt.getRoot()->getElement(), 49);
//        QCOMPARE(spt.Size(), current_count+1);

//    }

//    void SplayTreeRemove()
//    {
//        DDAD::SplayTree<int> spt = SampleSplayTree_int();
//        // Remove 2 from the tree
//        spt.remove(2);
//        // Tree should now remove 2 from the tree
//        QCOMPARE(spt.ContainsValue(2), false);
//        spt.insert(30);
//        spt.insert(20);
//        int current_count = spt.Size();
//        spt.remove(30);
//        QCOMPARE(spt.Size(), current_count-1);
//        current_count = spt.Size();
//        spt.remove(100000);
//        QCOMPARE(spt.Size(), current_count);
//    }

//    void SplayTreeFind()
//    {
//        DDAD::SplayTree<int> spt = SampleSplayTree_int();
//        // Find should splay an element to the top if it exists
//        spt.find(3);
//        QCOMPARE(spt.getRoot()->getElement(), 3);
//        // If not, it should default to greatest elt less than
//        spt.find(20);
//        QCOMPARE(spt.getRoot()->getElement(), 4);
//        // or least elt greater than
//        spt.find(-1);
//        QCOMPARE(spt.getRoot()->getElement(), 0);
//        spt.insert(30);
//        spt.find(1);
//        spt.find(49);
//        QCOMPARE(spt.getRoot()->getElement(), 30);
//    }

//    void SplayTreeMinMax()
//    {
//        DDAD::SplayTree<int> spt = SampleSplayTree_int();
//        //findMax should splay the maximum element to the root of the tree
//        spt.findMax();
//        QCOMPARE(spt.getRoot()->getElement(), 4);
//        //findMin should splay the minimum element to the root of the tree
//        spt.findMin();
//        QCOMPARE(spt.getRoot()->getElement(), 0);
//    }

//    void SplayTreeMerge()
//    {
//        DDAD::SplayTree<int> spt = DDAD::SplayTree<int>();
//        spt.insert(2);
//        spt.insert(4);
//        spt.insert(6);
//        spt.insert(7);
//        DDAD::SplayTree<int> spt2 = DDAD::SplayTree<int>();
//        spt2.insert(10);
//        spt2.insert(11);
//        spt2.insert(12);
//        // Merge the two trees together
//        spt.mergeTree(&spt2);
//        QCOMPARE(spt.getRoot()->getElement(), 7);
//        QCOMPARE(spt.getRoot()->right->getElement(), 12);
//        QCOMPARE(spt.ContainsValue(10), true);
//        QCOMPARE(spt.Size(), 7);

//        spt = DDAD::SplayTree<int>();
//        spt2 = DDAD::SplayTree<int>();
//        spt2.insert(2);
//        spt2.insert(3);
//        spt.mergeTree(&spt2);
//        QVERIFY(!spt.isEmpty());
//        QCOMPARE(spt.getRoot(), spt2.getRoot());

//    }

//    void SplayTreeSplit(){
//        DDAD::SplayTree<int> spt = SampleSplayTree_int();
//        DDAD::SplayTree<int> spt2 = DDAD::SplayTree<int>();
//        //SplitTree should return the subtree >
//        spt2 = spt.SplitTree(2);
//        QCOMPARE(spt2.ContainsValue(2), false);
//        QCOMPARE(spt2.ContainsValue(3), true);
//        QCOMPARE(spt2.ContainsValue(4), true);
//        QCOMPARE(spt.ContainsValue(2), true);
//        QCOMPARE(spt.ContainsValue(1), true);
//        QCOMPARE(spt.ContainsValue(3), false);
//        DDAD::SplayTree<int> spt3 = DDAD::SplayTree<int>();
//        spt3.insert(3);
//        spt3.insert(4);
//        spt2 = spt3.SplitTree(3);
//        QCOMPARE(spt3.ContainsValue(3), true);
//        QCOMPARE(spt3.ContainsValue(4), false);
//        spt = DDAD::SplayTree<int>();
//        spt2 = DDAD::SplayTree<int>();
//        spt.insert(1);
//        spt.insert(2);
//        spt2 = spt.SplitTree(2);
//        QCOMPARE(spt.ContainsValue(1), true);
//        QCOMPARE(spt.ContainsValue(2), false);
//    }

    //=========================================================================
    //      Arrangement: Tests
    //=========================================================================
    void BundleConstructor()
    {
        DDAD::Bundle bdl = DDAD::Bundle();
        QVERIFY(bdl.root_==nullptr);
    }

    void BundleSetNextPrev()
    {
        DDAD::SharedBundle sbdl1 =
                std::make_shared<DDAD::Bundle>(SampleBundle());
        DDAD::SharedBundle sbdl2 =
                std::make_shared<DDAD::Bundle>();
        sbdl2->Insert(SampleSharedSegment(2, 2, 3, 3, true));
        sbdl2->Insert(SampleSharedSegment(2, 2, 4, 4, true));
        sbdl1->next_bundle_ = sbdl2;
        sbdl2->prev_bundle_ = sbdl1;
        QVERIFY(sbdl1->next_bundle_ == sbdl2);
        DDAD::SharedBundle sbdl3 =
                std::make_shared<DDAD::Bundle>();
        sbdl3->Insert(SampleSharedSegment(0, 10, 3, 10, true));
        sbdl2->next_bundle_ = sbdl3;
        sbdl3->prev_bundle_ = sbdl2;
        QVERIFY(sbdl1->next_bundle_->next_bundle_==sbdl3);
        QVERIFY(sbdl3->prev_bundle_->prev_bundle_==sbdl1);
    }

    void BundleInsert()
    {
        DDAD::Bundle bdl = SampleBundle();
        int current_segments = bdl.Size();
        // Non-parallel segments
        bdl.Insert(SampleSharedSegment(0, 1, 11, 11, true));
        QCOMPARE(bdl.Size(), current_segments+1);
        bdl = SampleBundle();
        // Parallel segments
        bdl.Insert(SampleSharedSegment(0, 2, 5, 2, true));
        current_segments = bdl.Size();
        bdl.Insert(SampleSharedSegment(0, 4, 5, 4, true));
        QCOMPARE(bdl.Size(), current_segments+1);
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
        int current_count = bdl.Size();
        bdl.Remove(to_be_removed);
        QCOMPARE(bdl.Size(), current_count - 1);
    }

    void BundleContains()
    {
        DDAD::Bundle bdl = SampleBundle();
        bdl.Insert(SampleSharedSegment(-2, 2, 2, 2, true));
        DDAD::ArrangementVertex_2r test_vertex =
                SampleArrangementVertex(0, 1, true);
        QCOMPARE(bdl.Contains(test_vertex), true);

        DDAD::Bundle bdl2 = DDAD::Bundle();
        bdl2.Insert(SampleSharedSegment(3, 3, 10, 3, true));
        bdl2.Insert(SampleSharedSegment(5, 5, 10, 10, true));
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
        DDAD::Bundle bdl = DDAD::Bundle();
        bdl.Insert(SampleSharedSegment(1, 1, 15, 1, true));
        bdl.Insert(SampleSharedSegment(1, 5, 15, 5, true));
        bdl.Insert(SampleSharedSegment(1, 9, 15, 9, true));
        DDAD::ArrangementVertex_2r test_vertex =
                SampleArrangementVertex(15, 5, true);
        DDAD::RelativePosition rel_position =
                bdl.SetRelativePosition(test_vertex);
        QCOMPARE(rel_position, DDAD::ENDING);
        test_vertex = SampleArrangementVertex(10, 20, true);
        rel_position = bdl.SetRelativePosition(test_vertex);
        QCOMPARE(rel_position, DDAD::BELOW);
        test_vertex = SampleArrangementVertex(10, 0, false);
        rel_position = bdl.SetRelativePosition(test_vertex);
        QCOMPARE(rel_position, DDAD::ABOVE);
    }

    void BundleSize()
    {
        DDAD::Bundle bdl = DDAD::Bundle();
        bdl.Insert(SampleSharedSegment(1, 5, 5, 5, true));
        bdl.Insert(SampleSharedSegment(1, 6, 5, 6, true));
        QCOMPARE(bdl.Size(), 2);
        DDAD::Bundle bdl2 = DDAD::Bundle();
        QCOMPARE(bdl2.Size(), 0);
    }

    void BundleMerge()
    {
        DDAD::SharedBundle sbdl1 = std::make_shared<DDAD::Bundle>();
        sbdl1->Insert(SampleSharedSegment(0, 0, 20, 0, true));
        DDAD::SharedBundle sbdl2 = std::make_shared<DDAD::Bundle>();
        sbdl2->Insert(SampleSharedSegment(1, 1, 5, 2, true));
        sbdl1->next_bundle_ = sbdl2;
        sbdl2->prev_bundle_ = sbdl1;
        DDAD::SharedBundle sbdl3 = std::make_shared<DDAD::Bundle>();
        sbdl3->Insert(SampleSharedSegment(0, 4, 20, 4, false));
        sbdl2->next_bundle_ = sbdl3;
        sbdl3->prev_bundle_ = sbdl2;
        QVERIFY(sbdl3->prev_bundle_ == sbdl2);
        sbdl1->Merge(sbdl2);
        QVERIFY(sbdl1->top_segment_ == sbdl2->top_segment_);
        QVERIFY(sbdl3->prev_bundle_ == sbdl1);
        QVERIFY(sbdl3->next_bundle_ == nullptr);
        QVERIFY(sbdl1->next_bundle_ == sbdl3);
        QVERIFY(sbdl1->prev_bundle_ == nullptr);
        QVERIFY(sbdl1->root_->right == sbdl2->root_);
        QCOMPARE(sbdl1->Size(), 2);

        // Only two bundles in the list
        sbdl1 = SampleSharedBundle(SampleSharedSegment(2, 2, 8, 2, true));
        sbdl2 = SampleSharedBundle(SampleSharedSegment(2, 4, 8, 4, true));
        sbdl1->next_bundle_ = sbdl2;
        sbdl2->prev_bundle_ = sbdl1;
        sbdl1->Merge(sbdl2);
        QVERIFY(sbdl1->next_bundle_ == nullptr);
        QVERIFY(sbdl1->prev_bundle_ == nullptr);
        QVERIFY(sbdl1->root_->right == sbdl2->root_);
        QCOMPARE(sbdl1->Size(), 2);


    }

    void BundleSplit()
    {
        DDAD::SharedBundle bdl =
                SampleSharedBundle(SampleSharedSegment(4, 4, 10, 4, true),
                                   SampleSharedSegment(4, 14, 10, 14, true));
        DDAD::Point_2r split_here = DDAD::Point_2r(5, 5);
        int current_segments = bdl->Size();
        QCOMPARE(current_segments, 2);
        bdl->Split(split_here, bdl);
        QCOMPARE(bdl->Size(), current_segments - 1);
        QVERIFY(bdl->next_bundle_ != nullptr);
        QVERIFY(bdl->next_bundle_->prev_bundle_ == bdl);

        DDAD::SharedBundle bdl2 =
                SampleSharedBundle(SampleSharedSegment(4, 4, 10, 4, true));
        split_here = DDAD::Point_2r(10, 4);
        bdl2->Split(split_here, bdl2);
        QVERIFY(bdl2->Size() == 1);
        QVERIFY(bdl2->next_bundle_ == nullptr);
        QVERIFY(bdl2->prev_bundle_ == nullptr);

        DDAD::SharedBundle bdl3 =
                SampleSharedBundle(SampleSharedSegment(4, 4, 10, 4, true),
                                   SampleSharedSegment(4, 14, 10, 14, true));
        bdl3->Insert(SampleSharedSegment(4, 2, 10, 2, true));
        current_segments = bdl3->Size();
        QVERIFY(current_segments == 3);
        bdl3->Split(DDAD::Point_2r(5, 5), bdl3);
        QVERIFY(bdl3->Size() == 2);
        QVERIFY(bdl3->next_bundle_->Size() == 1);
        QVERIFY(bdl3->next_bundle_->next_bundle_ == nullptr);
        QVERIFY(bdl3->prev_bundle_ == nullptr);
        QVERIFY(bdl3->next_bundle_->prev_bundle_ == bdl3);

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
        bdt.Insert(bdlred);
        //try to insert a blue bundle
        bdt.Insert(bdlblue);
        QCOMPARE(bdt.root_->element, bdlred);
        //size of bdt should be 1, since the blue bundle should not insert
        QCOMPARE(bdt.Size(), 1);
        //insert another red bundle
        bdt.Insert(bdlred2);
        //size of bdt should be 2, now that we've added 2 red bundles
        QCOMPARE(bdt.Size(), 2);
        bdt.Insert(bdlred2);
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
        bdt.Insert(bdlred);
        //insert a second red bundle
        bdt.Insert(bdlred2);
        //remove first red bundle
        bdt.Remove(bdlred);
        //second red bundle should be at the root, size should be 1
        QCOMPARE(bdt.root_->element, bdlred2);
        QCOMPARE(bdt.Size(), 1);
    }

    void BundleTreeSplay()
    {
        DDAD::BundleTree bdt = DDAD::BundleTree();
        DDAD::SharedBundle bdlred = SampleSharedBundle();
        DDAD::SharedBundle bdlred2 =
                SampleSharedBundle(SampleSharedSegment(0, 4, 5, 4, true),
                                   SampleSharedSegment(1, 6, 3, 7, true));
        bdt.Insert(bdlred);
        bdt.Insert(bdlred2);

        DDAD::ArrangementVertex_2r vertex = SampleArrangementVertex(0,4,true);
        bdt.Splay(*vertex.get_point(), bdt.root_);
        QCOMPARE(bdt.root_->element,bdlred2);

    }

    void BundleTreeSplitAtVertex()
    {
        DDAD::BundleTree bdt = DDAD::BundleTree();
        DDAD::SharedBundle bdlblue =
                SampleSharedBundle(SampleSharedSegment(0, 4, 10, 4, false));
        DDAD::SharedBundle bdlblue2 =
                SampleSharedBundle(SampleSharedSegment(0, 10, 10, 10, false));
        DDAD::SharedBundle bdlred =
                SampleSharedBundle(SampleSharedSegment(0, 14, 5, 14, true),
                                   SampleSharedSegment(1, 16, 3, 17, true));
        DDAD::SharedBundle bdlred2 =
                SampleSharedBundle(SampleSharedSegment(0, 4, 5, 4, true),
                                   SampleSharedSegment(1, 6, 3, 7, true));
        DDAD::SharedBundle bdlred3 =
                SampleSharedBundle(SampleSharedSegment(0, 24, 5, 24, true),
                                   SampleSharedSegment(1, 26, 3, 27, true));
        bdt.InsertBundle(bdlred);
        bdt.InsertBundle(bdlred2);
        bdt.InsertBundle(bdlred3);
        bdlred2->set_next_bundle(bdlblue2);
        bdlred2->set_prev_bundle(bdlblue);
        DDAD::ArrangementVertex_2r vertex = SampleArrangementVertex(2,15,true);

        bdt.SplitBundleAtVertex(vertex);
        QCOMPARE(bdt.Size(), 4);
    }

    void BundleListGenerateSentinels()
    {
        DDAD::Arrangement_2r sample_arrangement = DDAD::Arrangement_2r();
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(4, 5), DDAD::Point_2r(6, 6), true);
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(3, 8), DDAD::Point_2r(10, 3), false);
        std::list<DDAD::ArrangementVertex_2r> L = sample_arrangement.get_vertices();
        DDAD::BundleList bdl = DDAD::BundleList();
        DDAD::BundleTree bdt = DDAD::BundleTree();
        bdl.GenerateSentinels(L, bdt);
        //QVERIFY(DDAD::Point_2r(3, 8) < bdl.get_top());
       // QVERIFY(DDAD::Point_2r(10, 3) > bdl.get_bottom_segment_);
        QVERIFY(bdl.top()->next_bundle() != nullptr);
        QVERIFY(bdl.bottom_segment_->prev_bundle() != nullptr);
    }

    void BundleListInsert()
    {
        DDAD::BundleList bdl = DDAD::BundleList();
        DDAD::SharedBundle bundle = SampleSharedBundle();
        DDAD::SharedBundle bundle2 =
                SampleSharedBundle(SampleSharedSegment(0, 1, 1, 2, false),
                                   SampleSharedSegment(0, 2, 3, 3, false));
        QVERIFY(bdl.bottom_segment_ == nullptr);
        QVERIFY(bdl.top() == nullptr);
        bdl.InsertBundle(bundle,nullptr);
        QCOMPARE(bdl.bottom_segment_, bundle);
        QCOMPARE(bdl.top(), bundle);
        bdl.InsertBundle(bundle2,bundle);
        QCOMPARE(bdl.bottom_segment_, bundle);
        QCOMPARE(bdl.top(), bundle2);
        QCOMPARE(bundle->next_bundle(), bundle2);
        QCOMPARE(bundle2->prev_bundle(), bundle);
        QVERIFY(bundle->prev_bundle() == nullptr);
        QVERIFY(bundle2->next_bundle() == nullptr);
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
        QCOMPARE(bdl.bottom_segment_, bundle2);
        QCOMPARE(bdl.top(), bundle2);

        // Test on a single-bundle list
        DDAD::BundleList bdl2 = DDAD::BundleList();
        DDAD::SharedBundle bundle1 = SampleSharedBundle();
        bdl2.InsertBundle(bundle1, nullptr);
        QCOMPARE(bdl2.top(), bundle1);
        bdl2.RemoveBundle(bundle1);
        QVERIFY(bdl2.top() == nullptr);

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
        QCOMPARE(bdl.bottom_segment_, bundle2);
        QCOMPARE(bdl.top(), bundle2);
        QVERIFY(bundle2->next_bundle() == nullptr);
        QVERIFY(bundle2->prev_bundle() == nullptr);

    }

    void BundleListLocateVertex()
    {
        DDAD::Arrangement_2r sample_arrangement = DDAD::Arrangement_2r();
        // Create bounding box
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(12, 9), DDAD::Point_2r(3, 3), true);
        DDAD::BundleList bdl = DDAD::BundleList();
        DDAD::BundleTree bdt = DDAD::BundleTree();
        bdl.GenerateSentinels(sample_arrangement.get_vertices(), bdt);
        // Add a bunch of segments
        DDAD::SharedBundle blue1 =
                SampleSharedBundle(SampleSharedSegment(3, 3, 12, 3, false));
        DDAD::SharedBundle blue2 =
                SampleSharedBundle(SampleSharedSegment(3, 8, 12, 8, false));
        DDAD::SharedBundle red1 =
                SampleSharedBundle(SampleSharedSegment(3, 5, 12, 5, true));
        DDAD::SharedBundle red2 =
                SampleSharedBundle(SampleSharedSegment(3, 9, 12, 9, true));
        DDAD::ArrangementVertex_2r on_r1 =
                SampleArrangementVertex(3, 5, true);
        DDAD::ArrangementVertex_2r on_b2 =
                SampleArrangementVertex(3, 8, false);
        DDAD::ArrangementVertex_2r between_b1_r1 =
                SampleArrangementVertex(9, 4, true);
        bdl.InsertBundle(blue1, bdl.bottom_segment_);
        bdl.InsertBundle(red1, blue1);
        bdl.InsertBundle(blue2, red1);
        bdl.InsertBundle(red2, blue2);
        bdt.InsertBundle(red1);
        bdt.InsertBundle(red2);
        // Tests
        DDAD::SharedBundle red_below, red_above, blue_below, blue_above;
        bdl.LocateVertex(on_r1, bdt, red_above, red_below,
                                 blue_above, blue_below);
        QVERIFY(red_below == red1->prev_bundle()->prev_bundle());
        QVERIFY(red_above == red2);
        QVERIFY(blue_above == blue2);
        QVERIFY(blue_below == blue1);

        bdl.LocateVertex(on_b2, bdt, red_above, red_below,
                         blue_above, blue_below);
        QVERIFY(red_below == red1);
        QVERIFY(red_above == red2);
        QVERIFY(blue_below == blue1);
        QVERIFY(blue_above == blue2->next_bundle()->next_bundle());

        bdl.LocateVertex(between_b1_r1, bdt, red_above, red_below,
                         blue_above, blue_below);
        QVERIFY(red_below == red1->prev_bundle()->prev_bundle());
        QVERIFY(red_above == red1);
        QVERIFY(blue_below == blue1);
        QVERIFY(blue_above == blue2);

        // Two segments that intersect
        sample_arrangement = DDAD::Arrangement_2r();
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(3, 3), DDAD::Point_2r(10, 8), true);
        bdl = DDAD::BundleList();
        bdt = DDAD::BundleTree();
        bdl.GenerateSentinels(sample_arrangement.get_vertices(), bdt);
        // Add a bunch of segments
        DDAD::ArrangementVertex_2r pt1 =
                SampleArrangementVertex(3, 8, false);
        bdl.LocateVertex(pt1, bdt, red_above, red_below, blue_above, blue_below);
        QVERIFY(red_below == bdl.bottom_segment_);
        QVERIFY(red_above == bdl.bottom_segment_->next_bundle()->next_bundle());
        QVERIFY(blue_below == bdl.top()->prev_bundle()->prev_bundle());
        QVERIFY(blue_above == bdl.top());


    }

    void BundleListSplitBundleAtVertex()
    {
        // Splitting at vertex in a bundle
        DDAD::BundleList bdl = DDAD::BundleList();
        DDAD::SharedBundle sbdl1 = std::make_shared<DDAD::Bundle>();
        sbdl1->Insert(SampleSharedSegment(1, 1, 20, 1, true));
        sbdl1->Insert(SampleSharedSegment(1, 5, 20, 5, true));
        bdl.InsertBundle(sbdl1, nullptr);
        bdl.SplitBundleAtVertex(
                    sbdl1, SampleArrangementVertex(5, 3, true));
        QVERIFY(bdl.bottom_segment_ == sbdl1);
        QCOMPARE(sbdl1->Size(), 1);
        QVERIFY(bdl.top() == sbdl1->next_bundle());
        QVERIFY(sbdl1->next_bundle()->prev_bundle() == sbdl1);

        // Splitting at vertex at an endpoint
        bdl = DDAD::BundleList();
        sbdl1 = std::make_shared<DDAD::Bundle>();
        sbdl1->Insert(SampleSharedSegment(1, 1, 20, 1, true));
        sbdl1->Insert(SampleSharedSegment(1, 5, 20, 5, true));
        sbdl1->Insert(SampleSharedSegment(1, 7, 20, 7, true));
        bdl.InsertBundle(sbdl1, nullptr);
        bdl.SplitBundleAtVertex(
                    sbdl1, SampleArrangementVertex(20, 5, true));
        QVERIFY(bdl.bottom_segment_ == sbdl1);
        QCOMPARE(sbdl1->Size(), 2);
        QVERIFY(bdl.top() == sbdl1->next_bundle());
        QVERIFY(sbdl1->next_bundle()->prev_bundle() == sbdl1);
        QCOMPARE(sbdl1->next_bundle()->Size(), 1);

        // Splitting with a bundle above
        bdl = DDAD::BundleList();
        sbdl1 = std::make_shared<DDAD::Bundle>();
        sbdl1->Insert(SampleSharedSegment(1, 1, 20, 1, true));
        sbdl1->Insert(SampleSharedSegment(1, 5, 20, 5, true));
        DDAD::SharedBundle sbdl2 = SampleSharedBundle();
        sbdl2->Insert(SampleSharedSegment(1, 7, 20, 7, false));
        bdl.InsertBundle(sbdl1, nullptr);
        bdl.InsertBundle(sbdl2, sbdl1);
        bdl.SplitBundleAtVertex(
                    sbdl1, SampleArrangementVertex(20, 5, true));
        QVERIFY(bdl.bottom_segment_ == sbdl1);
        QVERIFY(bdl.top() == sbdl2);
        QVERIFY(sbdl1->next_bundle() != sbdl2);
        QVERIFY(sbdl2->prev_bundle() != sbdl1);
        QCOMPARE(sbdl1->Size(), 1);
        QCOMPARE(sbdl2->prev_bundle()->Size(), 1);
    }

    void BundleListSplitBundlesContaining()
    {
        DDAD::Arrangement_2r sample_arrangement = DDAD::Arrangement_2r();
        // Create bounding box
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(100, 100), DDAD::Point_2r(3, 3), true);
        DDAD::BundleList bdl = DDAD::BundleList();
        DDAD::BundleTree bdt = DDAD::BundleTree();
        bdl.GenerateSentinels(sample_arrangement.get_vertices(), bdt);
        DDAD::SharedBundle bdl1 = std::make_shared<DDAD::Bundle>();
        bdl1->Insert(SampleSharedSegment(5, 5, 20, 5, true));
        bdl1->Insert(SampleSharedSegment(5, 10, 20, 10, true));
        bdt.InsertBundle(bdl1);
        bdl.InsertBundle(bdl1, bdl.bottom_segment_);
        DDAD::ArrangementVertex_2r test_pt =
                SampleArrangementVertex(6, 6, true);
        bdl.SplitBundlesContaining(test_pt, bdt, bdl.top(), bdl.bottom_segment_);
        QCOMPARE(bdl1->Size(), 1);
        QCOMPARE(bdl1->next_bundle()->Size(), 1);
        QVERIFY(bdl1->next_bundle() != nullptr);
        QVERIFY(bdl1->next_bundle()->next_bundle() != nullptr);
        QVERIFY(bdl1->prev_bundle() != nullptr);
        QVERIFY(bdl1->prev_bundle()->prev_bundle() != nullptr);
    }

    void BundleListSortPortion()
    {
        DDAD::Arrangement_2r sample_arrangement = DDAD::Arrangement_2r();
        // Create bounding box
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(100, 100), DDAD::Point_2r(2, 2), true);
        DDAD::BundleList bdl = DDAD::BundleList();
        DDAD::BundleTree bdt = DDAD::BundleTree();
        bdl.GenerateSentinels(sample_arrangement.get_vertices(), bdt);
        DDAD::SharedBundle blue =
                SampleSharedBundle(SampleSharedSegment(3, 3, 6, 6, false));
        DDAD::SharedBundle red =
                SampleSharedBundle(SampleSharedSegment(3, 8, 9, 2, true));
        int numIntersections = 0;

        bdl.InsertBundle(blue, bdl.bottom_segment_);
        bdl.InsertBundle(red, blue);
        bdt.InsertBundle(red);

        QVERIFY(red->prev_bundle() == blue);
        QVERIFY(blue->next_bundle() == red);
        QVERIFY(red->next_bundle() == bdl.top());
        QVERIFY(blue->prev_bundle() == bdl.bottom_segment_);
        QVERIFY(bdl.top()->prev_bundle() == red);
        QVERIFY(bdl.bottom_segment_->next_bundle() == blue);
        numIntersections +=
                bdl.SortPortion(SampleArrangementVertex(6, 6, true),
                                bdl.bottom_segment_, bdl.top());

        QVERIFY(red->next_bundle() == blue);
        QVERIFY(red->prev_bundle() == bdl.bottom_segment_);
        QVERIFY(blue->prev_bundle() == red);
        QVERIFY(blue->next_bundle() == bdl.top());
        QCOMPARE(numIntersections, 1);

//        bdl.RemoveBundle(blue);
//        bdl.InsertBundle(blue2, bdl.get_bottom());
//        bdl.MergeOrderedBundles(bdt);
//        numIntersections =
//                bdl.SortPortion(SampleArrangementVertex(8, 4, true),
//                                red2, blue);
//        QVERIFY(red2->get_next_bundle() == nullptr);
//        QVERIFY(blue ->get_prev_bundle() == nullptr);
//        QCOMPARE(numIntersections, 2);

//        // Swap red/blue
//        bdl = DDAD::BundleList();
//        blue =
//                SampleSharedBundle(SampleSharedSegment(3, 3, 6, 6, false));
//        DDAD::SharedBundle blue2 =
//                SampleSharedBundle(SampleSharedSegment(7, 2, 8, 4, false));
//        red =
//                SampleSharedBundle(SampleSharedSegment(3, 8, 9, 2, true));
//        numIntersections = 0;

//        bdl.InsertBundle(red, nullptr);
//        bdl.InsertBundle(blue, nullptr);

//        QVERIFY(blue->get_prev_bundle() == nullptr);
//        QVERIFY(red->get_next_bundle() == nullptr);
//        QVERIFY(blue->get_next_bundle() == red);
//        QVERIFY(red->get_prev_bundle() == blue);
//        QVERIFY(bdl.get_bottom() == blue);
//        QVERIFY(bdl.get_top() == red);
//        numIntersections +=
//                bdl.SortPortion(SampleArrangementVertex(6, 6, false),
//                                blue, red);

//        QVERIFY(blue->get_next_bundle() == nullptr);
//        QVERIFY(blue->get_prev_bundle() == red);
//        QVERIFY(red->get_next_bundle() == blue);
//        QVERIFY(red->get_prev_bundle() == nullptr);
//        QVERIFY(bdl.get_bottom() == red);
//        QVERIFY(bdl.get_top() == blue);
//        QCOMPARE(numIntersections, 1);

//        bdl.RemoveBundle(blue);
//        bdl.InsertBundle(blue2, nullptr);
//        numIntersections +=
//                bdl.SortPortion(SampleArrangementVertex(8, 4, false),
//                                blue2, red);
//        QVERIFY(blue2->get_next_bundle() == nullptr);
//        QVERIFY(red->get_prev_bundle() == nullptr);
//        QCOMPARE(numIntersections, 2);

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
        QVERIFY(red->next_bundle() == nullptr);
        QVERIFY(red->prev_bundle() == blue);
        QVERIFY(blue->prev_bundle() == nullptr);
        QVERIFY(blue->next_bundle() == red);
        QVERIFY(bdl.bottom_segment_ == blue);
        QVERIFY(bdl.top() == red);

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
        QVERIFY(bdl.bottom_segment_ == blue2);
        QVERIFY(bdl.top() == red2);
        bdl.SwapAdjacentBundles(red, blue);
        QVERIFY(bdl.bottom_segment_ == blue2);
        QVERIFY(bdl.top() == red2);
        QVERIFY(bdl.bottom_segment_->next_bundle() == blue);
        QVERIFY(bdl.top()->prev_bundle() == red);
    }

    void BundleListMergeOrderedBundles()
    {
        DDAD::Arrangement_2r sample_arrangement = DDAD::Arrangement_2r();
        // Create bounding box
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(100, 100), DDAD::Point_2r(3, 3), true);
        DDAD::BundleList bdl = DDAD::BundleList();
        DDAD::BundleTree bdt = DDAD::BundleTree();
        bdl.GenerateSentinels(sample_arrangement.vertices(), bdt);
        DDAD::SharedBundle blue1 = SampleSharedBundle(
                    SampleSharedSegment(5, 5, 10, 5, false));
        DDAD::SharedBundle blue2 = SampleSharedBundle(
                    SampleSharedSegment(10, 5, 10, 10, false));
        DDAD::SharedBundle original_bottom = bdl.bottom_segment_;
        DDAD::SharedBundle original_top = bdl.top();
        bdl.InsertBundle(blue1, bdl.bottom_segment_);
//        bdl.InsertBundle(blue2, blue1);
        QVERIFY(bdl.bottom_segment_ == original_bottom);
        QVERIFY(bdl.top() == original_top);
        bdl.MergeOrderedBundles(bdt);
        QVERIFY(bdl.bottom_segment_ == original_bottom);
        QVERIFY(bdl.top() != original_top);
        QVERIFY(bdl.bottom_segment_->next_bundle() == bdl.top());
    }

    void BundleListInsertLeftEndpoint()
    {
        DDAD::Arrangement_2r sample_arrangement = DDAD::Arrangement_2r();
        // Create bounding box
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(100, 100), DDAD::Point_2r(3, 3), true);
        DDAD::BundleList bdl = DDAD::BundleList();
        DDAD::BundleTree bdt = DDAD::BundleTree();
        bdl.GenerateSentinels(sample_arrangement.get_vertices(), bdt);
        DDAD::SharedPoint_2r pt1 = std::make_shared<DDAD::Point_2r>(3, 8);
        DDAD::SharedPoint_2r pt2 = std::make_shared<DDAD::Point_2r>(10, 3);
        DDAD::SharedSegment seg12 =
                std::make_shared<DDAD::Segment_2r_colored>(pt1, pt2, true);
        DDAD::ArrangementVertex_2r to_insert =
                DDAD::ArrangementVertex_2r(pt1, pt2, true, seg12);
        DDAD::SharedBundle original_bottom = bdl.bottom_segment_;
        DDAD::SharedBundle original_top = bdl.top();
        QVERIFY(bdl.bottom_segment_->next_bundle() == bdl.top());
        QVERIFY(bdl.top()->prev_bundle() == bdl.bottom_segment_);
        bdl.InsertLeftEndpoint(to_insert, bdt);
        QVERIFY(bdl.bottom_segment_->next_bundle() != bdl.top());
        QVERIFY(bdl.bottom_segment_->next_bundle()->next_bundle()
                == bdl.top());
        QVERIFY(bdl.bottom_segment_->next_bundle()->Contains(to_insert));
        QVERIFY(bdl.bottom_segment_ == original_bottom);
        QVERIFY(bdl.top() == original_top);

        // Have to return list to invariant state
        bdl.MergeOrderedBundles(bdt);
        // Add the next bundle
        pt1 = std::make_shared<DDAD::Point_2r>(4, 5);
        pt2 = std::make_shared<DDAD::Point_2r>(6, 6);
        seg12 = std::make_shared<DDAD::Segment_2r_colored>(pt1, pt2, false);
        DDAD::ArrangementVertex_2r to_insert2 =
                DDAD::ArrangementVertex_2r(pt1, pt2, false, seg12);
        bdl.InsertLeftEndpoint(to_insert2, bdt);
        QVERIFY(bdl.top() == original_top);
        QVERIFY(bdl.bottom_segment_ == original_bottom);
        QVERIFY(bdl.bottom_segment_->next_bundle()->Contains(to_insert2));
        QVERIFY(bdl.bottom_segment_->Contains(to_insert));
        QVERIFY(bdl.top()->prev_bundle()->Contains(to_insert2));
        QVERIFY(bdl.top()->prev_bundle()->prev_bundle()
                ->Contains(to_insert));
    }

    void BundleListRemoveRightEndpoint()
    {
        // Set up the state of the first InsertLeftEndpoint tes.t
        DDAD::Arrangement_2r sample_arrangement = DDAD::Arrangement_2r();
        // Create bounding box
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(100, 100), DDAD::Point_2r(3, 3), true);
        DDAD::BundleList bdl = DDAD::BundleList();
        DDAD::BundleTree bdt = DDAD::BundleTree();
        bdl.GenerateSentinels(sample_arrangement.get_vertices(), bdt);
        DDAD::SharedPoint_2r pt1 = std::make_shared<DDAD::Point_2r>(3, 8);
        DDAD::SharedPoint_2r pt2 = std::make_shared<DDAD::Point_2r>(10, 8);
        DDAD::SharedSegment seg12 =
                std::make_shared<DDAD::Segment_2r_colored>(pt1, pt2, true);
        DDAD::ArrangementVertex_2r to_insert =
                DDAD::ArrangementVertex_2r(pt1, pt2, true, seg12);
        bdl.InsertLeftEndpoint(to_insert, bdt);
        bdl.MergeOrderedBundles(bdt);
        DDAD::SharedPoint_2r pt3 = std::make_shared<DDAD::Point_2r>(4, 5);
        DDAD::SharedPoint_2r pt4 = std::make_shared<DDAD::Point_2r>(6, 6);
        DDAD::SharedSegment seg34 =
                std::make_shared<DDAD::Segment_2r_colored>(pt3, pt4, false);
        DDAD::ArrangementVertex_2r to_insert2 =
                DDAD::ArrangementVertex_2r(pt3, pt4, false, seg34);
        bdl.InsertLeftEndpoint(to_insert2, bdt);
        // When we get to the first right-endpoint, remove that segment
        DDAD::ArrangementVertex_2r to_remove =
                DDAD::ArrangementVertex_2r(pt2, pt1, true, seg12);
        int current_count = bdl.bottom_segment_->Size();
        bdl.RemoveRightEndpoint(to_remove, bdt);
        QCOMPARE(bdl.bottom_segment_->Size(), current_count-1);
        to_remove = DDAD::ArrangementVertex_2r(pt4, pt3, false, seg34);
        bdl.RemoveRightEndpoint(to_remove, bdt);
        QVERIFY(bdl.bottom_segment_->next_bundle() == bdl.top());
        QVERIFY(bdl.top()->prev_bundle() == bdl.bottom_segment_);
    }


    void CountIntersections()
    {
        // Simple arrangements
        std::cout << "\nTest: Two crossing segments... ";
        DDAD::Arrangement_2r sample_arrangement = DDAD::Arrangement_2r();
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(4, 6), DDAD::Point_2r(6, 7), true);
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(3, 8), DDAD::Point_2r(10, 3), false);
        int intersections = DDAD::CountIntersections(sample_arrangement);
        // Should just be two intersecting lines

        QCOMPARE(intersections, 1);

        std::cout << "\nTest: Three crossing segments...";
        // Add a second red line that also intersects with the blue line
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(7, 2), DDAD::Point_2r(9, 6), true);
        intersections = DDAD::CountIntersections(sample_arrangement);
        QCOMPARE(intersections, 2);

        std::cout << "\nTest: Three crossing segments and one floating...";
        // Add another blue line that does not intersect any existing lines
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(30, 30), DDAD::Point_2r(40, 40), true);
        intersections = DDAD::CountIntersections(sample_arrangement);
        QCOMPARE(intersections, 2);


        // Same arrangement, but with coloring order swapped
        std::cout << "\nTest: Two crossing segments (reversed colors)...";
        sample_arrangement = DDAD::Arrangement_2r();
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(4, 5), DDAD::Point_2r(6, 6), false);
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(3, 8), DDAD::Point_2r(10, 2), true);
        intersections = DDAD::CountIntersections(sample_arrangement);
        // Should just be two intersecting lines
        QCOMPARE(intersections, 1);

        // Add a second red line that also intersects with the blue line
        std::cout << "\nTest: Three crossing segments (reversed colors)...";
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(7, 2), DDAD::Point_2r(8, 4), false);
        intersections = DDAD::CountIntersections(sample_arrangement);
        QCOMPARE(intersections, 2);


        // Add another blue line that does not intersect any existing lines
        std::cout << "\nTest: Three crossing segments and one floating (reversed colors)...";
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(30, 30), DDAD::Point_2r(40, 40), false);
        intersections = DDAD::CountIntersections(sample_arrangement);
        QCOMPARE(intersections, 2);

        // Only red segments
        std::cout << "\nTest: Only red segments...";
        sample_arrangement = DDAD::Arrangement_2r();
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(2, 10), DDAD::Point_2r(10, 10), true);
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(2, 5), DDAD::Point_2r(10, 5), true);
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(2, 2), DDAD::Point_2r(10, 2), true);
        intersections = DDAD::CountIntersections(sample_arrangement);
        QCOMPARE(intersections, 0);

        // Small grid arrangement
        std::cout << "\nTest: Small grid arrangement...";
        sample_arrangement = DDAD::Arrangement_2r();
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(11, 14), DDAD::Point_2r(19, 12), false);
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(12, 15), DDAD::Point_2r(20, 14), false);
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(14, 20), DDAD::Point_2r(21, 15), false);
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(12, 11), DDAD::Point_2r(15, 21), true);
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(14, 11), DDAD::Point_2r(17, 19), true);
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(17, 11), DDAD::Point_2r(22, 19), true);
        intersections = DDAD::CountIntersections(sample_arrangement);
        QCOMPARE(intersections, 9);

        // Small grid arrangement with reversed colors
        std::cout << "\nTest: Small grid arrangement (reversed colors)...";
        sample_arrangement = DDAD::Arrangement_2r();
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(1, 4), DDAD::Point_2r(9, 2), true);
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(2, 5), DDAD::Point_2r(10, 4), true);
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(4, 10), DDAD::Point_2r(11, 5), true);
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(2, 1), DDAD::Point_2r(5, 11), false);
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(4, 1), DDAD::Point_2r(7, 9), false);
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(7, 1), DDAD::Point_2r(12, 9), false);
        intersections = DDAD::CountIntersections(sample_arrangement);
        QCOMPARE(intersections, 9);

        // Larger grid arrangement
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

        // Larger grid arrangement with reversed colors and non-vertical lines
        sample_arrangement = DDAD::Arrangement_2r();
        for(int ii = 2; ii < 22; ii++)
        {
            sample_arrangement.AddSegment(
                        DDAD::Point_2r(ii, 2), DDAD::Point_2r(ii, 24), false);
            sample_arrangement.AddSegment(
                        DDAD::Point_2r(2, ii), DDAD::Point_2r(24, ii), true);
        }
        intersections = DDAD::CountIntersections(sample_arrangement);
        QCOMPARE(intersections, 400);

        sample_arrangement = DDAD::Arrangement_2r();
        for(int ii = 2; ii < 22; ii++)
        {
            sample_arrangement.AddSegment(
                        DDAD::Point_2r(ii, 2), DDAD::Point_2r(ii+1, 24), false);
            sample_arrangement.AddSegment(
                        DDAD::Point_2r(2, ii), DDAD::Point_2r(24, ii+1), true);
        }
        sample_arrangement.AddSegment(DDAD::Point_2r(50, 15),
                                      DDAD::Point_2r(60, 20), true);
        intersections = DDAD::CountIntersections(sample_arrangement);
        QCOMPARE(intersections, 400);

        // Zig-zag arrangement
        sample_arrangement = DDAD::Arrangement_2r();
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(23, 19), DDAD::Point_2r(33, 15), true);
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(33, 15), DDAD::Point_2r(23, 8), true);
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(23, 8), DDAD::Point_2r(33, 6), true);
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(33, 6), DDAD::Point_2r(23, 1), true);
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(25, 22), DDAD::Point_2r(25, 0), false);
        sample_arrangement.AddSegment(
                    DDAD::Point_2r(29, 21), DDAD::Point_2r(29, 2), false);
        intersections = DDAD::CountIntersections(sample_arrangement);
        QCOMPARE(intersections, 8);
    }
};


QTEST_MAIN(tst_Geometry)
#include "tst_Geometry.moc"
