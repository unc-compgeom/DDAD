#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include "common.h"
#include "line.h"
#include <iostream>
#include <queue>


namespace DDAD{

class SplayNode;

typedef std::shared_ptr<Segment_2r_Colored> SharedSegment;
typedef std::shared_ptr<SplayNode> SharedNode;

class SplayNode
{
public:

    //Constructors

    SplayNode( ) : segment( nullptr ), left( nullptr ), right( nullptr ),
        left_root( nullptr ), right_root( nullptr ), top_seg( nullptr ),
        bot_seg( nullptr ) { }

    SplayNode(SharedSegment the_segment) : segment( the_segment ),
        left( nullptr ), right( nullptr ), left_root( nullptr ),
        right_root( nullptr ), top_seg( nullptr ), bot_seg( nullptr ) { }

    SplayNode(SharedSegment the_segment,
                SharedNode left,
                SharedNode right ) :
        segment( the_segment ), left( left ), right( right ),
        left_root( nullptr ), right_root( nullptr ), top_seg( nullptr ),
        bot_seg( nullptr ) { }

    //Destructor
    ~SplayNode(){MakeEmpty( );}


    //Accessors
    SharedNode get_root() {return root;}
    SharedSegment get_segment() {return segment;}
    SharedNode get_left() {return left;}
    SharedNode get_right() {return right;}
    SharedNode get_left_root() {return left_root;}
    SharedNode get_right_root() {return right_root;}
    SharedSegment get_top_seg() {return top_seg;}
    SharedSegment get_bot_seg() {return bot_seg;}


    //Tree methods
    void InsertSegment( SharedSegment x );
    void InsertBundle( SharedNode new_bundle );
    void Remove( SharedSegment x );
    void FindMin( );
    void FindMax( );
    void Find( SharedSegment x );
    bool is_empty( ) const;
    void MakeEmpty( );
    SharedNode SplitTree(SharedSegment x);
    void MergeTree(SharedNode R);
    const SplayNode & operator=( const SplayNode & rhs );
    void PrintBreadthFirst() const;
    void Print();
    void Print(SharedNode node, std::string space );


protected:
    static SharedNode tree_root = nullptr; //the top of the bundle tree
    SharedNode root;            //the top of the current bundle
    SharedSegment segment;      //pointer to the segment that the node contains
    SharedNode left;            //left and right nodes in bundle
    SharedNode right;
    SharedNode left_root;       //left and right nodes in bundle tree
    SharedNode right_root;
    SharedSegment top_seg;      //top and bottom segments in bundle
    SharedSegment bot_seg;

    SharedNode clone( SharedNode t ) const;

    void SplaySegment( SharedSegment pivot, SharedNode t );
    void SplayBundle( SharedSegment pivot, SharedNode t );
};



} //namespace DDAD

#endif // DATASTRUCTURES_H




























