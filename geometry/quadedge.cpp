/*
 * This file is part of DDAD.
 *
 * DDAD is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * DDAD is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details. You should have received a copy of the GNU General Public
 * License along with DDAD. If not, see <http://www.gnu.org/licenses/>.
 */

#include "quadedge.h"

namespace DDAD {

namespace QuadEdge {

//=============================================================================
// Interface: QuadEdge
//=============================================================================

class QuadEdge {
public:
    QuadEdge() {
        edges[0].index = 0;
        edges[1].index = 1;
        edges[2].index = 2;
        edges[3].index = 3;

        edges[0].next = edges+0;
        edges[1].next = edges+3;
        edges[2].next = edges+2;
        edges[3].next = edges+1;

        unsigned int id = Edge::nextID;

        edges[0].id = id+0;
        edges[1].id = id+1;
        edges[2].id = id+2;
        edges[3].id = id+3;

        Edge::nextID = id+4;
    }

    Edge edges[4];
};

//=============================================================================
// Implementation: Vertex
//=============================================================================

Vertex *Vertex::make(Cell *cell) {
    assert(cell != 0);

    return new Vertex(cell);
}

void Vertex::kill(Vertex *vertex) {
    assert(vertex != 0);

    delete vertex;
}

void Vertex::setID(unsigned int id) {
    assert(id > 0);

    this->id = id;
}

void Vertex::addEdge(Edge *edge) {
    assert(edge != 0);

    // only keep track of one edge in the orbit--this one is as good as any
    this->edge = edge;
}

void Vertex::removeEdge(Edge *edge) {
    assert(edge != 0);

    // replace the arbitrary edge with another edge in the orbit
    // use null if this is the only edge
    // assumes that the edge hasn't been actually removed yet

    Edge *next = edge->Onext();
    this->edge = next != edge ? next : 0;
}

Vertex::Vertex(Cell *cell) {
    assert(cell != 0);

    // FIXME
    /*
    this->pos[0] = 0.0;
    this->pos[1] = 0.0;
    this->pos[2] = 0.0;
    */
    this->cell   = cell;
    this->id     = cell->makeVertexID();
    this->data   = 0;
    this->edge   = 0;

    cell->addVertex(this);
}

Vertex::~Vertex() {
    cell->removeVertex(this);
}

//=============================================================================
// Implementation: Face
//=============================================================================

Face *Face::make(Cell *cell) {
    assert(cell != 0);

    return new Face(cell);
}

void Face::kill(Face *face) {
    assert(face != 0);

    delete face;
}

void Face::setID(unsigned int id) {
    assert(id > 0);

    this->id = id;
}

void Face::addEdge(Edge *edge) {
    assert(edge != 0);

    // only keep track of one edge in the orbit--this one is as good as any
    this->edge = edge;
}

void Face::removeEdge(Edge *edge) {
    assert(edge != 0);

    // replace the arbitrary edge with another edge in the orbit
    // use null if this is the only edge
    // assumes that the edge hasn't been actually removed yet
    Edge *next = edge->Onext();
    this->edge = next != edge ? next : 0;
}

Face::Face(Cell *cell) {
    assert(cell != 0);

    this->cell = cell;
    this->id   = cell->makeFaceID();
    this->data = 0;
    this->edge = 0;

    cell->addFace(this);
}

Face::~Face() {
    cell->removeFace(this);
}

//=============================================================================
// Implementation: Edge
//=============================================================================

Edge *Edge::make() {
    return (new QuadEdge())->edges;
}

void Edge::kill(Edge *edge) {
    assert(edge != 0);

    // detach the edge from its cell
    splice(edge, edge->Oprev());
    splice(edge->Sym(), edge->Sym()->Oprev());

    // free the quad edge that the edge belongs to
    delete (QuadEdge*)(edge-edge->index);
}

void Edge::splice(Edge *a, Edge *b) {
    assert(a != 0);
    assert(b != 0);

    // see Guibas and Stolfi
    Edge* alpha = a->Onext()->Rot();
    Edge* beta  = b->Onext()->Rot();

    Edge* t1 = b->Onext();
    Edge* t2 = a->Onext();
    Edge* t3 = beta->Onext();
    Edge* t4 = alpha->Onext();

    a->next     = t1;
    b->next     = t2;
    alpha->next = t3;
    beta->next  = t4;
}

void Edge::setID(unsigned int id) {
    assert(id > 0);

    this->id = id;
}

void Edge::setOrg(Vertex *org) {
    // add this edge to the (vertex) orbit of _org_
    vertex = org;
    org->addEdge(this);
}

void Edge::setDest(Vertex *dest) {
    // add this edge to the (vertex) orbit of _dest_
    Sym()->vertex = dest;
    dest->addEdge(Sym());
}

void Edge::setLeft(Face *left) {
    // add this edge to the (face) orbit of _left_
    Rot()->face = left;
    left->addEdge(this);
}

void Edge::setRight(Face *right) {
    // add this edge to the (face) orbit of _right_
    InvRot()->face = right;
    right->addEdge(Sym());
}

Edge::Edge() {
    // _index_ is initialized by QuadEdge
    // _next_ is initialized by QuadEdge
    // _id_ is initialized by QuadEdge
    data   = 0;
    vertex = 0;
    face   = 0;
}

Edge::~Edge() {}

unsigned int Edge::nextID = 4;

//=============================================================================
// Implementation: Cell
//=============================================================================

Cell *Cell::make() {
    // create a looping edge that connects to itself at a single vertex
    // the edge delimits two faces
    // this is the smallest cell that is consistent with our invariants

    Cell *cell = new Cell();

    Vertex *vertex = Vertex::make(cell);
    Face   *left   = Face::make(cell);
    Face   *right  = Face::make(cell);
    Edge   *edge   = Edge::make()->InvRot();

    edge->setOrg(vertex);
    edge->setDest(vertex);
    edge->setLeft(left);
    edge->setRight(right);

    return cell;
}

Cell *Cell::makeTetrahedron() {
    // use the Euler operators to construct a tetrahedron
    Cell *cell = make();

    // grab the initial vertex
    Vertex *vertex1;
    {
        CellVertexIterator iterator(cell);
        vertex1 = iterator.next();
        assert(vertex1 != 0);
    }

    // grab the initial edge and the initial faces
    Edge *edge1 = vertex1->getEdge();
    Face *left  = edge1->Left();
    Face *right = edge1->Right();

    // drop in four vertices along the initial edge
    Vertex *vertex2 = cell->makeVertexEdge(vertex1, left, right)->Dest();
    Vertex *vertex3 = cell->makeVertexEdge(vertex2, left, right)->Dest();
    Vertex *vertex4 = cell->makeVertexEdge(vertex3, left, right)->Dest();

    // cut each of the faces in half from complementary vertices
    Face *front  = cell->makeFaceEdge(left, vertex2, vertex4)->Right();
    Face *bottom = cell->makeFaceEdge(right, vertex1, vertex3)->Right();

    return cell;
}

void Cell::kill(Cell *cell) {
    assert(cell != 0);

    delete cell;
}

Edge *Cell::makeVertexEdge(Vertex *vertex, Face *left, Face *right) {
    assert(vertex != 0);
    assert(left != 0);
    assert(right != 0);

    // locate the edges to the right of each of the faces in the orbit of the
    // vertex
    Edge *edge  = vertex->getEdge();
    Edge *edge1 = getOrbitLeft(edge, right);
    Edge *edge2 = getOrbitLeft(edge, left);

    if (edge1 == 0) {
        (void)fprintf(stderr, "Cell::makeVertexEdge: unable to locate right face %u on vertex %u\n",
                      right->getID(), vertex->getID());
        abort();
    }

    if (edge2 == 0) {
        (void)fprintf(stderr, "Cell::makeVertexEdge: unable to locate left face %u on vertex %u\n",
                      left->getID(), vertex->getID());
        abort();
    }

    // create a new vertex and copy the position of the vertex of origin
    Vertex *vertexNew = Vertex::make(this);
    vertexNew->pos = vertex->pos;

    // create a new edge and rotate it to make a clockwise loop
    Edge *edgeNew = Edge::make()->Rot();

    // connect the origin (and destination) of the new edge to _vertex_ so that
    // the left face of the edge is _left_
    // this makes a loop on the inside of _left_
    Edge::splice(edge2, edgeNew);

    // split the origin and destination of the loop so that the right face of the
    // edge is now _right_
    // this results in a non-loop edge dividing _left_ from _right_
    Edge::splice(edge1, edgeNew->Sym());

    // initialize the secondary attributes of the new edge
    edgeNew->setOrg(edge1->Org());
    edgeNew->setLeft(edge2->Left());
    edgeNew->setRight(edge1->Left());

    // all edges leaving the destination orbit of the new edge now have the new
    // vertex as their vertex of origin
    setOrbitOrg(edgeNew->Sym(), vertexNew);

    return edgeNew;
}

void Cell::killVertexEdge(Edge *edge) {
    assert(edge != 0);

    // locate _edge1_ and _edge2_ as in _makeVertexEdge_
    Edge *edge1 = edge->Oprev();
    Edge *edge2 = edge->Lnext();

    // use _edge1_ for _edge2_ if the destination vertex is isolated
    if (edge2==edge->Sym()) {
        edge2 = edge1;
    }

    // inverse of _makeVertexEdge_
    Edge::splice(edge1, edge->Sym());
    Edge::splice(edge2, edge);

    // all edges leaving the destination orbit of the deleted edge now have its
    // origin vertex as their vertex of origin
    setOrbitOrg(edge2, edge1->Org());

    // don't use the deleted edge as a reference edge any more
    edge1->Org()->addEdge(edge1);
    edge1->Left()->addEdge(edge1);
    edge2->Left()->addEdge(edge2);

    // reclaim the vertex and the edge
    Vertex::kill(edge->Dest());
    Edge::kill(edge);
}

Edge *Cell::makeFaceEdge(Face *face, Vertex *org, Vertex *dest) {
    assert(face != 0);
    assert(org != 0);
    assert(dest != 0);

    // locate the edges leaving each of the vertices in the orbit of the face
    Edge *edge  = face->getEdge();
    Edge *edge1 = getOrbitOrg(edge, org);
    Edge *edge2 = getOrbitOrg(edge, dest);

    if (edge1 == 0) {
        (void)fprintf(stderr, "Cell::makeFaceEdge: unable to locate origin vertex %u on face %u\n",
                      org->getID(), face->getID());
        abort();
    }

    if (edge2 == 0) {
        (void)fprintf(stderr, "Cell::makeFaceEdge: unable to locate destination vertex %u on face %u\n",
                      dest->getID(), face->getID());
        abort();
    }

    // create a new face
    Face *faceNew = Face::make(this);

    // create a new (non-loop) edge
    Edge *edgeNew = Edge::make();

    // connect the destination of the new edge to the origin of _edge2_
    // both faces of the edge are now _face_
    Edge::splice(edge2, edgeNew->Sym());

    // connect the origin of the new edge to _edge1_
    // _face_ is split in half along the new edge, with the new face introduced
    // on the right
    Edge::splice(edge1, edgeNew);

    // initialize the secondary attributes of the new edge
    edgeNew->setOrg(edge1->Org());
    edgeNew->setDest(edge2->Org());
    edgeNew->setLeft(edge2->Left());

    // all edges in the right orbit of the new edge (i.e. the left orbit of its
    // Sym) now have the new face as their left face
    setOrbitLeft(edgeNew->Sym(), faceNew);

    return edgeNew;
}

void Cell::killFaceEdge(Edge *edge) {
    assert(edge != 0);

    // locate _edge1_ and _edge2_ as in _makeFaceEdge_
    Edge *edge1 = edge->Oprev();
    Edge *edge2 = edge->Lnext();

    // use _edge2_ for _edge1_ if the right face is inside a loop
    if (edge1 == edge->Sym()) {
        edge1 = edge2;
    }

    // inverse of _makeFaceEdge_
    Edge::splice(edge2, edge->Sym());
    Edge::splice(edge1, edge);

    // all edges in the right orbit of the deleted edge now have its left face
    // as their left face
    setOrbitLeft(edge1, edge2->Left());

    // don't use the deleted edge as a reference edge any more
    edge1->Org()->addEdge(edge1);
    edge2->Org()->addEdge(edge2);
    edge2->Left()->addEdge(edge2);

    // reclaim the face and the edge
    Face::kill(edge->Right());
    Edge::kill(edge);
}

void Cell::addVertex(Vertex *vertex) {
    assert(vertex != 0);

    // expand the vertex array, if necessary
    if (vertexCount >= vertexSize) {
        unsigned int vertexSizeNew = vertexSize*2;
        Vertex     **verticesNew   = new Vertex*[vertexSizeNew];

        // FIXME?
        for (unsigned int i = 0; i < vertexCount; i++) {
            verticesNew[i] = vertices[i];
        }

        delete[] vertices;

        vertices   = verticesNew;
        vertexSize = vertexSizeNew;
    }

    // add the vertex to the end of the array

    vertices[vertexCount++] = vertex;
}

void Cell::removeVertex(Vertex *vertex) {
    assert(vertex != 0);

    // locate the vertex in the array and replace it with the current last vertex
    // if already the last vertex, just overwrite it
    // slow: should make this a doubly-linked list ???
    // FIXME?
    for (unsigned int i = vertexCount; i > 0; i--) {
        if (vertices[i-1] == vertex) {
            vertices[i-1] = vertices[--vertexCount];
            return;
        }
    }

    assert(0); // ??
}

void Cell::addFace(Face *face) {
    assert(face != 0);

    // expand the face array, if necessary
    if (faceCount >= faceSize) {
        unsigned int faceSizeNew = faceSize*2;
        Face       **facesNew    = new Face*[faceSizeNew];

        // FIXME?
        for (unsigned int i = 0; i < faceCount; i++) {
            facesNew[i] = faces[i];
        }

        delete[] faces;

        faces    = facesNew;
        faceSize = faceSizeNew;
    }

    // add the face to the end of the array
    faces[faceCount++] = face;
}

void Cell::removeFace(Face *face) {
    assert(face != 0);

    // locate the face in the array and replace it with the current last face
    // if already the last face, just overwrite it
    // slow: should make this a doubly-linked list ???
    // FIXME?
    for (unsigned int i = faceCount; i > 0; i--) {
        if (faces[i-1] == face) {
            faces[i-1] = faces[--faceCount];
            return;
        }
    }

    assert(0); // ??
}

Cell::Cell() {
    // preallocate enough elements for a cube
    vertices    = new Vertex*[8];
    vertexCount = 0;
    vertexSize  = 8;
    vertexID    = 1;

    faces     = new Face*[6];
    faceCount = 0;
    faceSize  = 6;
    faceID    = 1;
}

Cell::~Cell() {
    // reclaim each of the vertices and faces still owned by the cell
    // go in backwards order so that when the elements try to remove themselves,
    // it will be linear time

    // FIXME ....?????
    {
        for (unsigned int i = vertexCount; i > 0; i--) {
            Vertex::kill(vertices[i-1]);
        }
    }

    {
        for (unsigned int i = faceCount; i > 0; i--) {
            Face::kill(faces[i-1]);
        }
    }

    // reclaim the vertex and face arrays
    delete[] vertices;
    delete[] faces;
}

Edge *Cell::getOrbitOrg(Edge *edge, Vertex *org) {
    assert(edge != 0);
    assert(org != 0);

    // traverse the Lnext orbit of _edge_ looking for an edge whose origin is
    // _org_
    Edge *scan = edge;

    do {
        if (scan->Org() == org) {
            return scan;
        }

        scan = scan->Lnext();
    } while (scan != edge);

    return 0;
}

void Cell::setOrbitOrg(Edge *edge, Vertex *org) {
    assert(edge != 0);
    assert(org != 0);

    // traverse the Onext orbit of _edge_, setting the origin of each edge to
    // _org_
    Edge *scan = edge;

    do {
        scan->setOrg(org);
        scan = scan->Onext();
    } while (scan != edge);
}

Edge *Cell::getOrbitLeft(Edge *edge, Face *left) {
    assert(edge != 0);
    assert(left != 0);

    // traverse the Onext orbit of _edge_ looking for an edge whose left face is
    // _left
    Edge *scan = edge;

    do {
        if (scan->Left() == left) {
            return scan;
        }

        scan = scan->Onext();
    } while (scan != edge);

    return 0;
}

void Cell::setOrbitLeft(Edge *edge, Face *left) {
    assert(edge != 0);
    assert(left != 0);

    // traverse the Lnext orbit of _edge_, setting the left face of each edge to
    // _left_
    Edge *scan = edge;

    do {
        scan->setLeft(left);
        scan = scan->Lnext();
    } while (scan != edge);
}

} // namespace QuadEdge

} // namespace DDAD
