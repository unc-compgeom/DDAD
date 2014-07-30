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
        int to_insert[] = {1, 2, 3, 4, 0};
        for(int ii = 0; ii < 5; ii++){
            spt.Insert(to_insert[ii]);
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
                                           tmp_segment->get_color(),
                                           tmp_segment);
        return vert;
    }

    DDAD::Bundle* SampleBundle()
    {
        DDAD::Bundle* bdl = new DDAD::Bundle();
        bdl->Insert(SampleSharedSegment(3, 3, 6, 6, true));
        return bdl;
    }

    DDAD::Bundle* SampleBundle(DDAD::SharedSegment seg1,
                              DDAD::SharedSegment seg2)
    {
        DDAD::Bundle* bdl = new DDAD::Bundle();
        bdl->Insert(seg1);
        bdl->Insert(seg2);
        return bdl;
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
        QCOMPARE(tmpNode.element, 1);

        // Try the copy constructor as well
        DDAD::BinaryNode<int> node2 =
                DDAD::BinaryNode<int>(2, &tmpNode, nullptr);
        QCOMPARE(node2.left->element, 1);
    }

    void SplayTreeConstructors()
    {
        DDAD::SplayTree<int> spt = DDAD::SplayTree<int>();
        QVERIFY(spt.root_==nullptr);

        spt = SampleSplayTree_int();
        DDAD::SplayTree<int> spt2 = spt;
        QCOMPARE(spt2.root_->element, spt.root_->element);
    }

    void SplayTreeInserts()
    {
        DDAD::SplayTree<int> spt = SampleSplayTree_int();
        // Inserting an element should splay to the top
        spt.Insert(20);
        QCOMPARE(spt.ContainsValue(20), true);
        QCOMPARE(spt.root_->element, 20);
        int current_count = spt.Size();
        spt.Insert(49);
        QCOMPARE(spt.ContainsValue(49), true);
        QCOMPARE(spt.root_->element, 49);
        QCOMPARE(spt.Size(), current_count+1);

    }

    void SplayTreeRemove()
    {
        DDAD::SplayTree<int> spt = SampleSplayTree_int();
        // Remove 2 from the tree
        spt.Remove(2);
        // Tree should now Remove 2 from the tree
        QCOMPARE(spt.ContainsValue(2), false);
        spt.Insert(30);
        spt.Insert(20);
        int current_count = spt.Size();
        spt.Remove(30);
        QCOMPARE(spt.Size(), current_count-1);
        current_count = spt.Size();
        spt.Remove(100000);
        QCOMPARE(spt.Size(), current_count);
    }

    void SplayTreeFind()
    {
        DDAD::SplayTree<int> spt = SampleSplayTree_int();
        // Find should splay an element to the top if it exists
        spt.Find(3);
        QCOMPARE(spt.root_->element, 3);
        // If not, it should default to greatest elt less than
        spt.Find(20);
        QCOMPARE(spt.root_->element, 4);
        // or least elt greater than
        spt.Find(-1);
        QCOMPARE(spt.root_->element, 0);
        spt.Insert(30);
        spt.Find(1);
        spt.Find(49);
        QCOMPARE(spt.root_->element, 30);
    }

    void SplayTreeMinMax()
    {
        DDAD::SplayTree<int> spt = SampleSplayTree_int();
        //FindMax should splay the maximum element to the root of the tree
        spt.FindMax();
        QCOMPARE(spt.root_->element, 4);
        //FindMin should splay the minimum element to the root of the tree
        spt.FindMin();
        QCOMPARE(spt.root_->element, 0);
    }

    void SplayTreeMerge()
    {
        DDAD::SplayTree<int> spt = DDAD::SplayTree<int>();
        spt.Insert(2);
        spt.Insert(4);
        spt.Insert(6);
        spt.Insert(7);
        DDAD::SplayTree<int> spt2 = DDAD::SplayTree<int>();
        spt2.Insert(10);
        spt2.Insert(11);
        spt2.Insert(12);
        // Merge the two trees together
        spt.MergeTree(&spt2);
        QCOMPARE(spt.root_->element, 7);
        QCOMPARE(spt.root_->right->element, 12);
        QCOMPARE(spt.ContainsValue(10), true);
        QCOMPARE(spt.Size(), 7);

        spt = DDAD::SplayTree<int>();
        spt2 = DDAD::SplayTree<int>();
        spt2.Insert(2);
        spt2.Insert(3);
        spt.MergeTree(&spt2);
        QVERIFY(!spt.is_empty());
        QCOMPARE(spt.root_, spt2.root_);

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
        spt3.Insert(3);
        spt3.Insert(4);
        spt2 = spt3.SplitTree(3);
        QCOMPARE(spt3.ContainsValue(3), true);
        QCOMPARE(spt3.ContainsValue(4), false);
        spt = DDAD::SplayTree<int>();
        spt2 = DDAD::SplayTree<int>();
        spt.Insert(1);
        spt.Insert(2);
        spt2 = spt.SplitTree(2);
        QCOMPARE(spt.ContainsValue(1), true);
        QCOMPARE(spt.ContainsValue(2), false);
    }

    //=========================================================================
    //      Arrangement: Tests
    //=========================================================================
    void BundleConstructor()
    {
        DDAD::Bundle bundle = DDAD::Bundle();
        QVERIFY(bundle.root_==nullptr);
    }

    void BundleSetNextPrev()
    {
        DDAD::Bundle* bundle1 = SampleBundle();
        DDAD::Bundle* bundle2 = SampleBundle(SampleSharedSegment(2, 2, 3, 3, true),
                                         SampleSharedSegment(2, 2, 4, 4, true));
        DDAD::Bundle* bundle3 = new DDAD::Bundle(
                    SampleSharedSegment(0, 10, 3, 10, true));
        bundle1->next_bundle_ = bundle2;
        bundle2->prev_bundle_ = bundle1;
        QVERIFY(bundle1->next_bundle_ == bundle2);
        bundle2->next_bundle_ = bundle3;
        bundle3->prev_bundle_ = bundle2;
        QVERIFY(bundle1->next_bundle_->next_bundle_==bundle3);
        QVERIFY(bundle3->prev_bundle_->prev_bundle_==bundle1);

        delete bundle1, bundle2, bundle3;
    }

    void BundleInsert()
    {
        DDAD::Bundle* bundle = SampleBundle();
        DDAD::Bundle* bundle2 = new DDAD::Bundle();
        DDAD::Bundle* bundle3 = new DDAD::Bundle();
        int current_segments = bundle->Size();
        // Non-parallel segments
        bundle->Insert(SampleSharedSegment(0, 1, 11, 11, true));
        QCOMPARE(bundle->Size(), current_segments+1);
        // Parallel segments
        bundle2->Insert(SampleSharedSegment(0, 2, 5, 2, true));
        current_segments = bundle2->Size();
        bundle2->Insert(SampleSharedSegment(0, 4, 5, 4, true));
        QCOMPARE(bundle2->Size(), current_segments+1);
        // Contains a point between two inserted segments
        bundle3->Insert(SampleSharedSegment(0, 2, 5, 2, true));
        bundle3->Insert(SampleSharedSegment(0, 5, 5, 5, true));
        QCOMPARE(bundle3->Contains(SampleArrangementVertex(3, 3, true)), true);
        delete bundle, bundle2, bundle3;
    }

    void BundleRemove()
    {
        DDAD::Bundle* bundle = new DDAD::Bundle();

        DDAD::SharedSegment to_be_removed =
                SampleSharedSegment(100, 100, 120, 120, true);
        bundle->Insert(to_be_removed);
        // Try to remove a segment
        int current_count = bundle->Size();
        bundle->Remove(to_be_removed);
        QCOMPARE(bundle->Size(), current_count - 1);

        DDAD::SharedSegment not_in_bundle =
                SampleSharedSegment(10, 10, 200, 200, true);
        current_count = bundle->Size();
        bundle->Remove(not_in_bundle);
        QCOMPARE(bundle->Size(), current_count);

        delete bundle;
    }

    void BundleContains()
    {
        DDAD::Bundle* bundle = SampleBundle();
        DDAD::Bundle* bundle2 = new DDAD::Bundle();

        bundle->Insert(SampleSharedSegment(-2, 2, 2, 2, true));
        DDAD::ArrangementVertex_2r test_vertex =
                SampleArrangementVertex(0, 1, true);
        QCOMPARE(bundle->Contains(test_vertex), true);

        bundle2->Insert(SampleSharedSegment(3, 3, 10, 3, true));
        bundle2->Insert(SampleSharedSegment(5, 5, 10, 10, true));
        test_vertex = SampleArrangementVertex(2, 2, true);
        QCOMPARE(bundle2->Contains(test_vertex), false);
        test_vertex = SampleArrangementVertex(6, 4, true);
        QCOMPARE(bundle2->Contains(test_vertex), true);
        test_vertex = SampleArrangementVertex(5, 5, true);
        QCOMPARE(bundle2->Contains(test_vertex), true);
        test_vertex = SampleArrangementVertex(10, 10, true);
        QCOMPARE(bundle2->Contains(test_vertex), true);
        test_vertex = SampleArrangementVertex(3, 3, true);
        QCOMPARE(bundle2->Contains(test_vertex), true);
        test_vertex = SampleArrangementVertex(10, 3, true);
        QCOMPARE(bundle2->Contains(test_vertex), true);

        delete bundle, bundle2;
    }

    void BundleRelativePosition()
    {
        DDAD::Bundle* bundle = new DDAD::Bundle();

        bundle->Insert(SampleSharedSegment(1, 1, 15, 1, true));
        bundle->Insert(SampleSharedSegment(1, 5, 15, 5, true));
        bundle->Insert(SampleSharedSegment(1, 9, 15, 9, true));
        DDAD::ArrangementVertex_2r test_vertex =
                SampleArrangementVertex(15, 5, true);
        DDAD::RelativePosition rel_position =
                bundle->SetRelativePosition(test_vertex);
        QCOMPARE(rel_position, DDAD::ENDING);
        test_vertex = SampleArrangementVertex(10, 20, true);
        rel_position = bundle->SetRelativePosition(test_vertex);
        QCOMPARE(rel_position, DDAD::BELOW);
        test_vertex = SampleArrangementVertex(10, 0, false);
        rel_position = bundle->SetRelativePosition(test_vertex);
        QCOMPARE(rel_position, DDAD::ABOVE);

        delete bundle;
    }

    void BundleSize()
    {
        DDAD::Bundle* bundle = new DDAD::Bundle();
        DDAD::Bundle* bundle2 = new DDAD::Bundle();

        bundle->Insert(SampleSharedSegment(1, 5, 5, 5, true));
        bundle->Insert(SampleSharedSegment(1, 6, 5, 6, true));
        QCOMPARE(bundle->Size(), 2);
        QCOMPARE(bundle2->Size(), 0);

        delete bundle, bundle2;
    }

    void BundleMerge()
    {
        DDAD::Bundle* bundle1 = new DDAD::Bundle();
        DDAD::Bundle* bundle2 = new DDAD::Bundle();
        DDAD::Bundle* bundle3 = new DDAD::Bundle();


        bundle1->Insert(SampleSharedSegment(0, 0, 20, 0, true));
        bundle2->Insert(SampleSharedSegment(1, 1, 5, 2, true));
        bundle1->next_bundle_ = bundle2;
        bundle2->prev_bundle_ = bundle1;
        bundle3->Insert(SampleSharedSegment(0, 4, 20, 4, false));
        bundle2->next_bundle_ = bundle3;
        bundle3->prev_bundle_ = bundle2;
        QVERIFY(bundle3->prev_bundle_ == bundle2);
        bundle1->Merge(bundle2);
        QVERIFY(bundle1->top_segment_ == bundle2->top_segment_);
        QVERIFY(bundle3->prev_bundle_ == bundle1);
        QVERIFY(bundle3->next_bundle_ == nullptr);
        QVERIFY(bundle1->next_bundle_ == bundle3);
        QVERIFY(bundle1->prev_bundle_ == nullptr);
        QVERIFY(bundle1->root_->right == bundle2->root_);
        QCOMPARE(bundle1->Size(), 2);

        delete bundle1, bundle2, bundle3;

        // Only two bundles in the list
        DDAD::Bundle* bundle4 = new DDAD::Bundle(SampleSharedSegment(2, 2, 8, 2, true));
        DDAD::Bundle* bundle5 = new DDAD::Bundle(SampleSharedSegment(2, 4, 8, 4, true));

        bundle4->next_bundle_ = bundle5;
        bundle5->prev_bundle_ = bundle4;
        bundle4->Merge(bundle5);
        QVERIFY(bundle4->next_bundle_ == nullptr);
        QVERIFY(bundle4->prev_bundle_ == nullptr);
        QVERIFY(bundle4->root_->right == bundle5->root_);
        QCOMPARE(bundle4->Size(), 2);

        delete bundle4, bundle5;
    }

    void BundleSplit()
    {
        DDAD::Bundle* bundle1 =
                SampleBundle(SampleSharedSegment(4, 4, 10, 4, true),
                             SampleSharedSegment(4, 14, 10, 14, true));
        DDAD::Bundle* bundle2 = new DDAD::Bundle(SampleSharedSegment(4, 4, 10, 4, true));

        DDAD::Point_2r split_here = DDAD::Point_2r(5, 5);
        int current_segments = bundle1->Size();
        QCOMPARE(current_segments, 2);
        bundle1->Split(split_here);
        QCOMPARE(bundle1->Size(), current_segments - 1);
        QVERIFY(bundle1->next_bundle_ != nullptr);
        QVERIFY(bundle1->next_bundle_->prev_bundle_ == bundle1);

        split_here = DDAD::Point_2r(10, 4);
        bundle2->Split(split_here);
        QVERIFY(bundle2->Size() == 1);
        QVERIFY(bundle2->next_bundle_ == nullptr);
        QVERIFY(bundle2->prev_bundle_ == nullptr);

        delete bundle1, bundle2;

        DDAD::Bundle* bundle3 =
                SampleBundle(SampleSharedSegment(4, 4, 10, 4, true),
                             SampleSharedSegment(4, 14, 10, 14, true));
        bundle3->Insert(SampleSharedSegment(4, 2, 10, 2, true));
        current_segments = bundle3->Size();
        QVERIFY(current_segments == 3);
        bundle3->Split(DDAD::Point_2r(5, 5));
        QVERIFY(bundle3->Size() == 2);
        QVERIFY(bundle3->next_bundle_->Size() == 1);
        QVERIFY(bundle3->next_bundle_->next_bundle_ == nullptr);
        QVERIFY(bundle3->prev_bundle_ == nullptr);
        QVERIFY(bundle3->next_bundle_->prev_bundle_ == bundle3);

        delete bundle3;
    }

    void BundleTreeInsert()
    {
        DDAD::BundleTree bdt = DDAD::BundleTree();
        DDAD::Bundle* redbundle1 =
                SampleBundle(SampleSharedSegment(1, 1, 7, 2, true),
                             SampleSharedSegment(1, 2, 8, 3, true));
        DDAD::Bundle* redbundle2 =
                SampleBundle(SampleSharedSegment(1, 4, 7, 4, true),
                             SampleSharedSegment(1, 7, 3, 7, true));
        DDAD::Bundle* bluebundle1 =
                SampleBundle(SampleSharedSegment(1, 5, 10, 5, false),
                             SampleSharedSegment(1, 6, 6, 6, false));

        //insert a red bundle
        bdt.Insert(redbundle1);
        //try to insert a blue bundle
        bdt.Insert(bluebundle1);
        QCOMPARE(bdt.root_->element, redbundle1);
        //size of bdt should be 1, since the blue bundle should not insert
        QCOMPARE(bdt.Size(), 1);
        //insert another red bundle
        bdt.Insert(redbundle2);
        //size of bdt should be 2, now that we've added 2 red bundles
        QCOMPARE(bdt.Size(), 2);
        bdt.Insert(redbundle2);
        //shouldn't be able to insert duplicates
        QCOMPARE(bdt.Size(), 2);

        delete redbundle1, redbundle2, bluebundle1;
    }

    void BundleTreeRemove()
    {
        DDAD::BundleTree bdt = DDAD::BundleTree();
        DDAD::Bundle* redbundle1 =
                SampleBundle(SampleSharedSegment(1, 1, 7, 2, true),
                             SampleSharedSegment(1, 2, 8, 3, true));
        DDAD::Bundle* redbundle2 =
                SampleBundle(SampleSharedSegment(1, 4, 7, 4, true),
                             SampleSharedSegment(1, 7, 3, 7, true));

        //insert a red bundle
        bdt.Insert(redbundle1);
        //insert a second red bundle
        bdt.Insert(redbundle2);
        //remove first red bundle
        bdt.Remove(redbundle1);
        //second red bundle should be at the root, size should be 1
        QCOMPARE(bdt.root_->element, redbundle2);
        QCOMPARE(bdt.Size(), 1);

        delete redbundle1, redbundle2;
    }

    void BundleTreeSplay()
    {
        DDAD::BundleTree bdt = DDAD::BundleTree();
        DDAD::Bundle* redbundle1 = new DDAD::Bundle(
                    SampleSharedSegment(1, 10, 15, 10, true));
        DDAD::Bundle* redbundle2 =
                SampleBundle(SampleSharedSegment(0, 4, 5, 4, true),
                                   SampleSharedSegment(1, 6, 3, 7, true));
        bdt.Insert(redbundle1);
        bdt.Insert(redbundle2);

        DDAD::ArrangementVertex_2r vertex = SampleArrangementVertex(0, 4, true);
        bdt.Splay(*vertex.get_point(), bdt.root_);
        QCOMPARE(bdt.root_->element, redbundle2);
        delete redbundle1, redbundle2;
    }

    void BundleTreeSplitAtVertex()
    {
        DDAD::BundleTree bdt = DDAD::BundleTree();
        DDAD::Bundle* bluebundle1 =
                new DDAD::Bundle(SampleSharedSegment(0, 4, 10, 4, false));
        DDAD::Bundle* bluebundle2 =
                new DDAD::Bundle(SampleSharedSegment(0, 10, 10, 10, false));
        DDAD::Bundle* redbundle1 =
                SampleBundle(SampleSharedSegment(0, 14, 5, 14, true),
                                   SampleSharedSegment(1, 16, 3, 17, true));
        DDAD::Bundle* redbundle2 =
                SampleBundle(SampleSharedSegment(0, 4, 5, 4, true),
                                   SampleSharedSegment(1, 6, 3, 7, true));
        DDAD::Bundle* redbundle3 =
                SampleBundle(SampleSharedSegment(0, 24, 5, 24, true),
                                   SampleSharedSegment(1, 26, 3, 27, true));
        bdt.Insert(redbundle2);
        bdt.Insert(redbundle1);
        bdt.Insert(redbundle3);
        redbundle2->next_bundle_ = (bluebundle2);
        redbundle2->prev_bundle_ = (bluebundle1);
        DDAD::ArrangementVertex_2r vertex = SampleArrangementVertex(2,15,true);
        bdt.SplitAtVertex(vertex);
        QCOMPARE(bdt.Size(), 4);

        delete redbundle1, redbundle2, redbundle3, bluebundle1, bluebundle2;
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
        QVERIFY(bdl.top_->next_bundle_ != nullptr);
        QVERIFY(bdl.bottom_->prev_bundle_ != nullptr);
        QCOMPARE(bdl.top_->get_color(), false);
        QCOMPARE(bdl.bottom_->get_color(), true);
    }

    void BundleListInsert()
    {
        DDAD::BundleList bdl = DDAD::BundleList();
        DDAD::Bundle* bundle1 = new DDAD::Bundle();
        DDAD::Bundle* bundle2 =
                SampleBundle(SampleSharedSegment(0, 1, 1, 2, false),
                                   SampleSharedSegment(0, 2, 3, 3, false));
        QVERIFY(bdl.bottom_ == nullptr);
        QVERIFY(bdl.top_ == nullptr);
        bdl.Insert(bundle1,nullptr);
        QCOMPARE(bdl.bottom_, bundle1);
        QCOMPARE(bdl.top_, bundle1);
        bdl.Insert(bundle2,bundle1);
        QCOMPARE(bdl.bottom_, bundle1);
        QCOMPARE(bdl.top_, bundle2);
        QCOMPARE(bundle1->next_bundle_, bundle2);
        QCOMPARE(bundle2->prev_bundle_, bundle1);
        QVERIFY(bundle1->prev_bundle_ == nullptr);
        QVERIFY(bundle2->next_bundle_ == nullptr);

        delete bundle1, bundle2;
    }

    void BundleListRemove()
    {
        DDAD::BundleList bdl = DDAD::BundleList();
        DDAD::Bundle* bundle1 = new DDAD::Bundle(
                    SampleSharedSegment(0, 0, 2, 2, true));
        DDAD::Bundle* bundle2 =
                SampleBundle(SampleSharedSegment(0, 1, 1, 2, false),
                             SampleSharedSegment(0, 2, 3, 3, false));
        bdl.Insert(bundle1, nullptr);
        bdl.Insert(bundle2, bundle1);

        bdl.Remove(bundle1);
        QCOMPARE(bdl.bottom_, bundle2);
        QCOMPARE(bdl.top_, bundle2);

        delete bundle2;

        // Test on a single-bundle list
        DDAD::BundleList bdl2 = DDAD::BundleList();
        DDAD::Bundle* bundle3 = new DDAD::Bundle(
                    SampleSharedSegment(3, 3, 5, 5, true));
        bdl2.Insert(bundle3, nullptr);
        QCOMPARE(bdl2.top_, bundle3);
        bdl2.Remove(bundle3);
        QVERIFY(bdl2.top_ == nullptr);

        // Swap colors
        bdl = DDAD::BundleList();
        DDAD::Bundle* bundle4 = SampleBundle(SampleSharedSegment(1, 1, 2, 2, false),
                                    SampleSharedSegment(1, 5, 3, 2, false));
        DDAD::Bundle* bundle5 =
                SampleBundle(SampleSharedSegment(0, 1, 1, 2, true),
                                   SampleSharedSegment(0, 2, 3, 3, true));
        bdl.Insert(bundle4, nullptr);
        bdl.Insert(bundle5, bundle4);

        bdl.Remove(bundle4);
        QCOMPARE(bdl.bottom_, bundle5);
        QCOMPARE(bdl.top_, bundle5);
        QVERIFY(bundle5->next_bundle_ == nullptr);
        QVERIFY(bundle5->prev_bundle_ == nullptr);

        delete bundle5;
    }

//    void BundleListLocateVertex()
//    {
//        DDAD::Arrangement_2r sample_arrangement = DDAD::Arrangement_2r();
//        // Create bounding box
//        sample_arrangement.AddSegment(
//                    DDAD::Point_2r(12, 9), DDAD::Point_2r(3, 3), true);
//        DDAD::BundleList bdl = DDAD::BundleList();
//        DDAD::BundleTree bdt = DDAD::BundleTree();
//        bdl.GenerateSentinels(sample_arrangement.get_vertices(), bdt);
//        // Add a bunch of segments
//        DDAD::SharedBundle blue1 =
//                SampleSharedBundle(SampleSharedSegment(3, 3, 12, 3, false));
//        DDAD::SharedBundle blue2 =
//                SampleSharedBundle(SampleSharedSegment(3, 8, 12, 8, false));
//        DDAD::SharedBundle red1 =
//                SampleSharedBundle(SampleSharedSegment(3, 5, 12, 5, true));
//        DDAD::SharedBundle red2 =
//                SampleSharedBundle(SampleSharedSegment(3, 9, 12, 9, true));
//        DDAD::ArrangementVertex_2r on_r1 =
//                SampleArrangementVertex(3, 5, true);
//        DDAD::ArrangementVertex_2r on_b2 =
//                SampleArrangementVertex(3, 8, false);
//        DDAD::ArrangementVertex_2r between_b1_r1 =
//                SampleArrangementVertex(9, 4, true);
//        bdl.InsertBundle(blue1, bdl.bottom_);
//        bdl.InsertBundle(red1, blue1);
//        bdl.InsertBundle(blue2, red1);
//        bdl.InsertBundle(red2, blue2);
//        bdt.Insert(red1);
//        bdt.Insert(red2);
//        // Tests
//        DDAD::SharedBundle red_below, red_above, blue_below, blue_above;
//        bdl.LocateVertex(on_r1, bdt, red_above, red_below,
//                                 blue_above, blue_below);
//        QVERIFY(red_below == red1->prev_bundle_->prev_bundle_);
//        QVERIFY(red_above == red2);
//        QVERIFY(blue_above == blue2);
//        QVERIFY(blue_below == blue1);

//        bdl.LocateVertex(on_b2, bdt, red_above, red_below,
//                         blue_above, blue_below);
//        QVERIFY(red_below == red1);
//        QVERIFY(red_above == red2);
//        QVERIFY(blue_below == blue1);
//        QVERIFY(blue_above == blue2->next_bundle_->next_bundle_);

//        bdl.LocateVertex(between_b1_r1, bdt, red_above, red_below,
//                         blue_above, blue_below);
//        QVERIFY(red_below == red1->prev_bundle_->prev_bundle_);
//        QVERIFY(red_above == red1);
//        QVERIFY(blue_below == blue1);
//        QVERIFY(blue_above == blue2);

//        // Two segments that intersect
//        sample_arrangement = DDAD::Arrangement_2r();
//        sample_arrangement.AddSegment(
//                    DDAD::Point_2r(3, 3), DDAD::Point_2r(10, 8), true);
//        bdl = DDAD::BundleList();
//        bdt = DDAD::BundleTree();
//        bdl.GenerateSentinels(sample_arrangement.get_vertices(), bdt);
//        // Add a bunch of segments
//        DDAD::ArrangementVertex_2r pt1 =
//                SampleArrangementVertex(3, 8, false);
//        bdl.LocateVertex(pt1, bdt, red_above, red_below, blue_above, blue_below);
//        QVERIFY(red_below == bdl.bottom_);
//        QVERIFY(red_above == bdl.bottom_->next_bundle_->next_bundle_);
//        QVERIFY(blue_below == bdl.top_->prev_bundle_->prev_bundle_);
//        QVERIFY(blue_above == bdl.top_);


//    }

//    void BundleListSplitBundleAtVertex()
//    {
//        // Splitting at vertex in a bundle
//        DDAD::BundleList bdl = DDAD::BundleList();
//        DDAD::SharedBundle sbdl1 = std::make_shared<DDAD::Bundle>();
//        sbdl1->Insert(SampleSharedSegment(1, 1, 20, 1, true));
//        sbdl1->Insert(SampleSharedSegment(1, 5, 20, 5, true));
//        bdl.InsertBundle(sbdl1, nullptr);
//        bdl.SplitBundleAtVertex(
//                    sbdl1, SampleArrangementVertex(5, 3, true));
//        QVERIFY(bdl.bottom_ == sbdl1);
//        QCOMPARE(sbdl1->Size(), 1);
//        QVERIFY(bdl.top_ == sbdl1->next_bundle_);
//        QVERIFY(sbdl1->next_bundle_->prev_bundle_ == sbdl1);

//        // Splitting at vertex at an endpoint
//        bdl = DDAD::BundleList();
//        sbdl1 = std::make_shared<DDAD::Bundle>();
//        sbdl1->Insert(SampleSharedSegment(1, 1, 20, 1, true));
//        sbdl1->Insert(SampleSharedSegment(1, 5, 20, 5, true));
//        sbdl1->Insert(SampleSharedSegment(1, 7, 20, 7, true));
//        bdl.InsertBundle(sbdl1, nullptr);
//        bdl.SplitBundleAtVertex(
//                    sbdl1, SampleArrangementVertex(20, 5, true));
//        QVERIFY(bdl.bottom_ == sbdl1);
//        QCOMPARE(sbdl1->Size(), 2);
//        QVERIFY(bdl.top_ == sbdl1->next_bundle_);
//        QVERIFY(sbdl1->next_bundle_->prev_bundle_ == sbdl1);
//        QCOMPARE(sbdl1->next_bundle_->Size(), 1);

//        // Splitting with a bundle above
//        bdl = DDAD::BundleList();
//        sbdl1 = std::make_shared<DDAD::Bundle>();
//        sbdl1->Insert(SampleSharedSegment(1, 1, 20, 1, true));
//        sbdl1->Insert(SampleSharedSegment(1, 5, 20, 5, true));
//        DDAD::SharedBundle sbdl2 = SampleSharedBundle();
//        sbdl2->Insert(SampleSharedSegment(1, 7, 20, 7, false));
//        bdl.InsertBundle(sbdl1, nullptr);
//        bdl.InsertBundle(sbdl2, sbdl1);
//        bdl.SplitBundleAtVertex(
//                    sbdl1, SampleArrangementVertex(20, 5, true));
//        QVERIFY(bdl.bottom_ == sbdl1);
//        QVERIFY(bdl.top_ == sbdl2);
//        QVERIFY(sbdl1->next_bundle_ != sbdl2);
//        QVERIFY(sbdl2->prev_bundle_ != sbdl1);
//        QCOMPARE(sbdl1->Size(), 1);
//        QCOMPARE(sbdl2->prev_bundle_->Size(), 1);
//    }

//    void BundleListSplitBundlesContaining()
//    {
//        DDAD::Arrangement_2r sample_arrangement = DDAD::Arrangement_2r();
//        // Create bounding box
//        sample_arrangement.AddSegment(
//                    DDAD::Point_2r(100, 100), DDAD::Point_2r(3, 3), true);
//        DDAD::BundleList bdl = DDAD::BundleList();
//        DDAD::BundleTree bdt = DDAD::BundleTree();
//        bdl.GenerateSentinels(sample_arrangement.get_vertices(), bdt);
//        DDAD::SharedBundle bdl1 = std::make_shared<DDAD::Bundle>();
//        bdl1->Insert(SampleSharedSegment(5, 5, 20, 5, true));
//        bdl1->Insert(SampleSharedSegment(5, 10, 20, 10, true));
//        bdt.Insert(bdl1);
//        bdl.InsertBundle(bdl1, bdl.bottom_);
//        DDAD::ArrangementVertex_2r test_pt =
//                SampleArrangementVertex(6, 6, true);
//        bdl.SplitBundlesContaining(test_pt, bdt, bdl.top_, bdl.bottom_);
//        QCOMPARE(bdl1->Size(), 1);
//        QCOMPARE(bdl1->next_bundle_->Size(), 1);
//        QVERIFY(bdl1->next_bundle_ != nullptr);
//        QVERIFY(bdl1->next_bundle_->next_bundle_ != nullptr);
//        QVERIFY(bdl1->prev_bundle_ != nullptr);
//        QVERIFY(bdl1->prev_bundle_->prev_bundle_ != nullptr);
//    }

//    void BundleListSwapAdjacentBundles()
//    {


////        // Try with only two bundles
//        DDAD::BundleList bdl = DDAD::BundleList();
//        DDAD::SharedBundle red =
//                SampleSharedBundle(SampleSharedSegment(3, 3, 6, 6, true));
//        DDAD::SharedBundle blue =
//                SampleSharedBundle(SampleSharedSegment(3, 8, 9, 2, false));
////        bdl.InsertBundle(blue, nullptr);
////        bdl.InsertBundle(red, nullptr);
////        bdl.SwapAdjacentBundles(red, blue);
////        QVERIFY(red->next_bundle_ == nullptr);
////        QVERIFY(red->prev_bundle_ == blue);
////        QVERIFY(blue->prev_bundle_ == nullptr);
////        QVERIFY(blue->next_bundle_ == red);
////        QVERIFY(bdl.bottom_ == blue);
////        QVERIFY(bdl.top_ == red);

//        // Try a longer list of bundles
//        DDAD::Arrangement_2r sample_arrangement = DDAD::Arrangement_2r();
//                // Create bounding box
//                sample_arrangement.AddSegment(
//                            DDAD::Point_2r(100, 100), DDAD::Point_2r(2, 2), true);
//                bdl = DDAD::BundleList();
//                DDAD::BundleTree bdt = DDAD::BundleTree();
//                bdl.GenerateSentinels(sample_arrangement.get_vertices(), bdt);
//        red = SampleSharedBundle(SampleSharedSegment(3, 3, 6, 6, true));
//        DDAD::SharedBundle red2 =
//                SampleSharedBundle(SampleSharedSegment(3, 9, 7, 9, true));
//        blue = SampleSharedBundle(SampleSharedSegment(3, 8, 9, 2, false));
//        DDAD::SharedBundle blue2 =
//                SampleSharedBundle(SampleSharedSegment(3, 3, 5, 4, false));
//        bdl.InsertBundle(blue2, bdl.bottom_);
//        bdl.InsertBundle(red, blue2);
//        bdl.InsertBundle(blue, red);
//        bdl.InsertBundle(red2, blue);
//        QVERIFY(bdl.bottom_->next_bundle_ == blue2);
//        QVERIFY(bdl.top_->prev_bundle_ == red2);
//        QVERIFY(blue->prev_bundle_ == red);
//        QVERIFY(red->next_bundle_ == blue);
//        bdl.SwapAdjacentBundles(red, blue);
//        QVERIFY(bdl.bottom_->next_bundle_ == blue2);
//        QVERIFY(bdl.top_->prev_bundle_ == red2);
//        QVERIFY(blue->next_bundle_ == red);
//        QVERIFY(red->prev_bundle_ == blue);
//        QVERIFY(red->next_bundle_->prev_bundle_ == red);
//        QVERIFY(blue->next_bundle_->prev_bundle_ == blue);
//        QVERIFY(red->prev_bundle_->next_bundle_ == red);
//        QVERIFY(blue->prev_bundle_->next_bundle_ == blue);
//    }

//    void BundleListSortPortion()
//    {
//        DDAD::Arrangement_2r sample_arrangement = DDAD::Arrangement_2r();
//        // Create bounding box
//        sample_arrangement.AddSegment(
//                    DDAD::Point_2r(100, 100), DDAD::Point_2r(2, 2), true);
//        DDAD::BundleList bdl = DDAD::BundleList();
//        DDAD::BundleTree bdt = DDAD::BundleTree();
//        bdl.GenerateSentinels(sample_arrangement.get_vertices(), bdt);
//        DDAD::SharedBundle blue =
//                SampleSharedBundle(SampleSharedSegment(3, 3, 6, 6, false));
//        DDAD::SharedBundle red =
//                SampleSharedBundle(SampleSharedSegment(3, 8, 9, 2, true));
//        int numIntersections = 0;

//        bdl.InsertBundle(blue, bdl.bottom_);
//        bdl.InsertBundle(red, blue);
//        bdt.Insert(red);

//        QVERIFY(red->prev_bundle_ == blue);
//        QVERIFY(blue->next_bundle_ == red);
//        QVERIFY(red->next_bundle_ == bdl.top_);
//        QVERIFY(blue->prev_bundle_ == bdl.bottom_);
//        QVERIFY(bdl.top_->prev_bundle_ == red);
//        QVERIFY(bdl.bottom_->next_bundle_ == blue);
//        numIntersections +=
//                bdl.SortPortion(SampleArrangementVertex(6, 6, true),
//                                bdl.bottom_, bdl.top_);

//        QVERIFY(red->next_bundle_ == blue);
//        QVERIFY(red->prev_bundle_ == bdl.bottom_);
//        QVERIFY(blue->prev_bundle_ == red);
//        QVERIFY(blue->next_bundle_ == bdl.top_);
//        QCOMPARE(numIntersections, 1);

////        bdl.RemoveBundle(blue);
////        bdl.InsertBundle(blue2, bdl.get_bottom());
////        bdl.MergeOrderedBundles(bdt);
////        numIntersections =
////                bdl.SortPortion(SampleArrangementVertex(8, 4, true),
////                                red2, blue);
////        QVERIFY(red2->get_next_bundle_ == nullptr);
////        QVERIFY(blue ->prev_bundle_ == nullptr);
////        QCOMPARE(numIntersections, 2);

////        // Swap red/blue
////        bdl = DDAD::BundleList();
////        blue =
////                SampleSharedBundle(SampleSharedSegment(3, 3, 6, 6, false));
////        DDAD::SharedBundle blue2 =
////                SampleSharedBundle(SampleSharedSegment(7, 2, 8, 4, false));
////        red =
////                SampleSharedBundle(SampleSharedSegment(3, 8, 9, 2, true));
////        numIntersections = 0;

////        bdl.InsertBundle(red, nullptr);
////        bdl.InsertBundle(blue, nullptr);

////        QVERIFY(blue->prev_bundle_ == nullptr);
////        QVERIFY(red->get_next_bundle_ == nullptr);
////        QVERIFY(blue->get_next_bundle_ == red);
////        QVERIFY(red->prev_bundle_ == blue);
////        QVERIFY(bdl.get_bottom() == blue);
////        QVERIFY(bdl.get_top() == red);
////        numIntersections +=
////                bdl.SortPortion(SampleArrangementVertex(6, 6, false),
////                                blue, red);

////        QVERIFY(blue->get_next_bundle_ == nullptr);
////        QVERIFY(blue->prev_bundle_ == red);
////        QVERIFY(red->get_next_bundle_ == blue);
////        QVERIFY(red->prev_bundle_ == nullptr);
////        QVERIFY(bdl.get_bottom() == red);
////        QVERIFY(bdl.get_top() == blue);
////        QCOMPARE(numIntersections, 1);

////        bdl.RemoveBundle(blue);
////        bdl.InsertBundle(blue2, nullptr);
////        numIntersections +=
////                bdl.SortPortion(SampleArrangementVertex(8, 4, false),
////                                blue2, red);
////        QVERIFY(blue2->get_next_bundle_ == nullptr);
////        QVERIFY(red->prev_bundle_ == nullptr);
////        QCOMPARE(numIntersections, 2);

//    }

//    void BundleListMergeOrderedBundles()
//    {
//        DDAD::Arrangement_2r sample_arrangement = DDAD::Arrangement_2r();
//        // Create bounding box
//        sample_arrangement.AddSegment(
//                    DDAD::Point_2r(100, 100), DDAD::Point_2r(3, 3), true);
//        DDAD::BundleList bdl = DDAD::BundleList();
//        DDAD::BundleTree bdt = DDAD::BundleTree();
//        bdl.GenerateSentinels(sample_arrangement.get_vertices(), bdt);
//        DDAD::SharedBundle blue1 = SampleSharedBundle(
//                    SampleSharedSegment(5, 5, 10, 5, false));
//        DDAD::SharedBundle blue2 = SampleSharedBundle(
//                    SampleSharedSegment(10, 5, 10, 10, false));
//        DDAD::SharedBundle original_bottom = bdl.bottom_;
//        DDAD::SharedBundle original_top = bdl.top_;
//        bdl.InsertBundle(blue1, bdl.bottom_);
////        bdl.InsertBundle(blue2, blue1);
//        QVERIFY(bdl.bottom_ == original_bottom);
//        QVERIFY(bdl.top_ == original_top);
//        bdl.MergeOrderedBundles(bdt);
//        QVERIFY(bdl.bottom_ == original_bottom);
//        QVERIFY(bdl.top_ != original_top);
//        QVERIFY(bdl.bottom_->next_bundle_ == bdl.top_);
//    }

//    void BundleListInsertLeftEndpoint()
//    {
//        DDAD::Arrangement_2r sample_arrangement = DDAD::Arrangement_2r();
//        // Create bounding box
//        sample_arrangement.AddSegment(
//                    DDAD::Point_2r(100, 100), DDAD::Point_2r(3, 3), true);
//        DDAD::BundleList bdl = DDAD::BundleList();
//        DDAD::BundleTree bdt = DDAD::BundleTree();
//        bdl.GenerateSentinels(sample_arrangement.get_vertices(), bdt);
//        DDAD::SharedPoint_2r pt1 = std::make_shared<DDAD::Point_2r>(3, 8);
//        DDAD::SharedPoint_2r pt2 = std::make_shared<DDAD::Point_2r>(10, 3);
//        DDAD::SharedSegment seg12 =
//                std::make_shared<DDAD::Segment_2r_colored>(pt1, pt2, true);
//        DDAD::ArrangementVertex_2r to_insert =
//                DDAD::ArrangementVertex_2r(pt1, pt2, true, seg12);
//        DDAD::SharedBundle original_bottom = bdl.bottom_;
//        DDAD::SharedBundle original_top = bdl.top_;
//        QVERIFY(bdl.bottom_->next_bundle_ == bdl.top_);
//        QVERIFY(bdl.top_->prev_bundle_ == bdl.bottom_);
//        bdl.InsertLeftEndpoint(to_insert, bdt);
//        QVERIFY(bdl.bottom_->next_bundle_ != bdl.top_);
//        QVERIFY(bdl.bottom_->next_bundle_->next_bundle_
//                == bdl.top_);
//        QVERIFY(bdl.bottom_->next_bundle_->Contains(to_insert));
//        QVERIFY(bdl.bottom_ == original_bottom);
//        QVERIFY(bdl.top_ == original_top);

//        // Have to return list to invariant state
//        bdl.MergeOrderedBundles(bdt);
//        // Add the next bundle
//        pt1 = std::make_shared<DDAD::Point_2r>(4, 5);
//        pt2 = std::make_shared<DDAD::Point_2r>(6, 6);
//        seg12 = std::make_shared<DDAD::Segment_2r_colored>(pt1, pt2, false);
//        DDAD::ArrangementVertex_2r to_insert2 =
//                DDAD::ArrangementVertex_2r(pt1, pt2, false, seg12);
//        bdl.InsertLeftEndpoint(to_insert2, bdt);
//        QVERIFY(bdl.top_ == original_top);
//        QVERIFY(bdl.bottom_ == original_bottom);
//        QVERIFY(bdl.bottom_->next_bundle_->Contains(to_insert2));
//        QVERIFY(bdl.bottom_->Contains(to_insert));
//        QVERIFY(bdl.top_->prev_bundle_->Contains(to_insert2));
//        QVERIFY(bdl.top_->prev_bundle_->prev_bundle_
//                ->Contains(to_insert));
//    }

//    void BundleListRemoveRightEndpoint()
//    {
//        // Set up the state of the first InsertLeftEndpoint tes.t
//        DDAD::Arrangement_2r sample_arrangement = DDAD::Arrangement_2r();
//        // Create bounding box
//        sample_arrangement.AddSegment(
//                    DDAD::Point_2r(100, 100), DDAD::Point_2r(3, 3), true);
//        DDAD::BundleList bdl = DDAD::BundleList();
//        DDAD::BundleTree bdt = DDAD::BundleTree();
//        bdl.GenerateSentinels(sample_arrangement.get_vertices(), bdt);
//        DDAD::SharedPoint_2r pt1 = std::make_shared<DDAD::Point_2r>(3, 8);
//        DDAD::SharedPoint_2r pt2 = std::make_shared<DDAD::Point_2r>(10, 8);
//        DDAD::SharedSegment seg12 =
//                std::make_shared<DDAD::Segment_2r_colored>(pt1, pt2, true);
//        DDAD::ArrangementVertex_2r to_insert =
//                DDAD::ArrangementVertex_2r(pt1, pt2, true, seg12);
//        bdl.InsertLeftEndpoint(to_insert, bdt);
//        bdl.MergeOrderedBundles(bdt);
//        DDAD::SharedPoint_2r pt3 = std::make_shared<DDAD::Point_2r>(4, 5);
//        DDAD::SharedPoint_2r pt4 = std::make_shared<DDAD::Point_2r>(6, 6);
//        DDAD::SharedSegment seg34 =
//                std::make_shared<DDAD::Segment_2r_colored>(pt3, pt4, false);
//        DDAD::ArrangementVertex_2r to_insert2 =
//                DDAD::ArrangementVertex_2r(pt3, pt4, false, seg34);
//        bdl.InsertLeftEndpoint(to_insert2, bdt);
//        // When we get to the first right-endpoint, remove that segment
//        DDAD::ArrangementVertex_2r to_remove =
//                DDAD::ArrangementVertex_2r(pt2, pt1, true, seg12);
//        int current_count = bdl.bottom_->Size();
//        bdl.RemoveRightEndpoint(to_remove, bdt);
//        QCOMPARE(bdl.bottom_->Size(), current_count-1);
//        to_remove = DDAD::ArrangementVertex_2r(pt4, pt3, false, seg34);
//        bdl.RemoveRightEndpoint(to_remove, bdt);
//        QVERIFY(bdl.bottom_->next_bundle_ == bdl.top_);
//        QVERIFY(bdl.top_->prev_bundle_ == bdl.bottom_);
//    }


//    void CountIntersections()
//    {
//        // Simple arrangements
//        std::cout << "\nTest: Two crossing segments... ";
//        DDAD::Arrangement_2r sample_arrangement = DDAD::Arrangement_2r();
//        sample_arrangement.AddSegment(
//                    DDAD::Point_2r(4, 6), DDAD::Point_2r(6, 7), true);
//        sample_arrangement.AddSegment(
//                    DDAD::Point_2r(3, 8), DDAD::Point_2r(10, 3), false);
//        int intersections = DDAD::CountIntersections(sample_arrangement);
//        // Should just be two intersecting lines

//        QCOMPARE(intersections, 1);

//        std::cout << "\nTest: Three crossing segments...";
//        // Add a second red line that also intersects with the blue line
//        sample_arrangement.AddSegment(
//                    DDAD::Point_2r(7, 2), DDAD::Point_2r(9, 6), true);
//        intersections = DDAD::CountIntersections(sample_arrangement);
//        QCOMPARE(intersections, 2);

//        std::cout << "\nTest: Three crossing segments and one floating...";
//        // Add another blue line that does not intersect any existing lines
//        sample_arrangement.AddSegment(
//                    DDAD::Point_2r(30, 30), DDAD::Point_2r(40, 40), true);
//        intersections = DDAD::CountIntersections(sample_arrangement);
//        QCOMPARE(intersections, 2);


//        // Same arrangement, but with coloring order swapped
//        std::cout << "\nTest: Two crossing segments (reversed colors)...";
//        sample_arrangement = DDAD::Arrangement_2r();
//        sample_arrangement.AddSegment(
//                    DDAD::Point_2r(4, 5), DDAD::Point_2r(6, 6), false);
//        sample_arrangement.AddSegment(
//                    DDAD::Point_2r(3, 8), DDAD::Point_2r(10, 2), true);
//        intersections = DDAD::CountIntersections(sample_arrangement);
//        // Should just be two intersecting lines
//        QCOMPARE(intersections, 1);

//        // Add a second red line that also intersects with the blue line
//        std::cout << "\nTest: Three crossing segments (reversed colors)...";
//        sample_arrangement.AddSegment(
//                    DDAD::Point_2r(7, 2), DDAD::Point_2r(8, 4), false);
//        intersections = DDAD::CountIntersections(sample_arrangement);
//        QCOMPARE(intersections, 2);


//        // Add another blue line that does not intersect any existing lines
//        std::cout << "\nTest: Three crossing segments and one floating (reversed colors)...";
//        sample_arrangement.AddSegment(
//                    DDAD::Point_2r(30, 30), DDAD::Point_2r(40, 40), false);
//        intersections = DDAD::CountIntersections(sample_arrangement);
//        QCOMPARE(intersections, 2);

//        // Only red segments
//        std::cout << "\nTest: Only red segments...";
//        sample_arrangement = DDAD::Arrangement_2r();
//        sample_arrangement.AddSegment(
//                    DDAD::Point_2r(2, 10), DDAD::Point_2r(10, 10), true);
//        sample_arrangement.AddSegment(
//                    DDAD::Point_2r(2, 5), DDAD::Point_2r(10, 5), true);
//        sample_arrangement.AddSegment(
//                    DDAD::Point_2r(2, 2), DDAD::Point_2r(10, 2), true);
//        intersections = DDAD::CountIntersections(sample_arrangement);
//        QCOMPARE(intersections, 0);

//        // Small grid arrangement
//        std::cout << "\nTest: Small grid arrangement...";
//        sample_arrangement = DDAD::Arrangement_2r();
//        sample_arrangement.AddSegment(
//                    DDAD::Point_2r(11, 14), DDAD::Point_2r(19, 12), false);
//        sample_arrangement.AddSegment(
//                    DDAD::Point_2r(12, 15), DDAD::Point_2r(20, 14), false);
//        sample_arrangement.AddSegment(
//                    DDAD::Point_2r(14, 20), DDAD::Point_2r(21, 15), false);
//        sample_arrangement.AddSegment(
//                    DDAD::Point_2r(12, 11), DDAD::Point_2r(15, 21), true);
//        sample_arrangement.AddSegment(
//                    DDAD::Point_2r(14, 11), DDAD::Point_2r(17, 19), true);
//        sample_arrangement.AddSegment(
//                    DDAD::Point_2r(17, 11), DDAD::Point_2r(22, 19), true);
//        intersections = DDAD::CountIntersections(sample_arrangement);
//        QCOMPARE(intersections, 9);

//        // Small grid arrangement with reversed colors
//        std::cout << "\nTest: Small grid arrangement (reversed colors)...";
//        sample_arrangement = DDAD::Arrangement_2r();
//        sample_arrangement.AddSegment(
//                    DDAD::Point_2r(1, 4), DDAD::Point_2r(9, 2), true);
//        sample_arrangement.AddSegment(
//                    DDAD::Point_2r(2, 5), DDAD::Point_2r(10, 4), true);
//        sample_arrangement.AddSegment(
//                    DDAD::Point_2r(4, 10), DDAD::Point_2r(11, 5), true);
//        sample_arrangement.AddSegment(
//                    DDAD::Point_2r(2, 1), DDAD::Point_2r(5, 11), false);
//        sample_arrangement.AddSegment(
//                    DDAD::Point_2r(4, 1), DDAD::Point_2r(7, 9), false);
//        sample_arrangement.AddSegment(
//                    DDAD::Point_2r(7, 1), DDAD::Point_2r(12, 9), false);
//        intersections = DDAD::CountIntersections(sample_arrangement);
//        QCOMPARE(intersections, 9);

//        // Larger grid arrangement
//        sample_arrangement = DDAD::Arrangement_2r();
//        for(int ii = 2; ii < 22; ii++)
//        {
//            sample_arrangement.AddSegment(
//                        DDAD::Point_2r(ii, 1), DDAD::Point_2r(ii, 24), true);
//            sample_arrangement.AddSegment(
//                        DDAD::Point_2r(1, ii), DDAD::Point_2r(24, ii), false);
//        }
//        intersections = DDAD::CountIntersections(sample_arrangement);
//        QCOMPARE(intersections, 400);

//        // Larger grid arrangement with reversed colors and non-vertical lines
//        sample_arrangement = DDAD::Arrangement_2r();
//        for(int ii = 2; ii < 22; ii++)
//        {
//            sample_arrangement.AddSegment(
//                        DDAD::Point_2r(ii, 2), DDAD::Point_2r(ii, 24), false);
//            sample_arrangement.AddSegment(
//                        DDAD::Point_2r(2, ii), DDAD::Point_2r(24, ii), true);
//        }
//        intersections = DDAD::CountIntersections(sample_arrangement);
//        QCOMPARE(intersections, 400);

//        sample_arrangement = DDAD::Arrangement_2r();
//        for(int ii = 2; ii < 22; ii++)
//        {
//            sample_arrangement.AddSegment(
//                        DDAD::Point_2r(ii, 2), DDAD::Point_2r(ii+1, 24), false);
//            sample_arrangement.AddSegment(
//                        DDAD::Point_2r(2, ii), DDAD::Point_2r(24, ii+1), true);
//        }
//        sample_arrangement.AddSegment(DDAD::Point_2r(50, 15),
//                                      DDAD::Point_2r(60, 20), true);
//        intersections = DDAD::CountIntersections(sample_arrangement);
//        QCOMPARE(intersections, 400);

//        // Zig-zag arrangement
//        sample_arrangement = DDAD::Arrangement_2r();
//        sample_arrangement.AddSegment(
//                    DDAD::Point_2r(23, 19), DDAD::Point_2r(33, 15), true);
//        sample_arrangement.AddSegment(
//                    DDAD::Point_2r(33, 15), DDAD::Point_2r(23, 8), true);
//        sample_arrangement.AddSegment(
//                    DDAD::Point_2r(23, 8), DDAD::Point_2r(33, 6), true);
//        sample_arrangement.AddSegment(
//                    DDAD::Point_2r(33, 6), DDAD::Point_2r(23, 1), true);
//        sample_arrangement.AddSegment(
//                    DDAD::Point_2r(25, 22), DDAD::Point_2r(25, 0), false);
//        sample_arrangement.AddSegment(
//                    DDAD::Point_2r(29, 21), DDAD::Point_2r(29, 2), false);
//        intersections = DDAD::CountIntersections(sample_arrangement);
//        QCOMPARE(intersections, 8);
//    }
};


QTEST_MAIN(tst_Geometry)
#include "tst_Geometry.moc"
