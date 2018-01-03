use std::collections::HashMap;
use key::{KeyMaker, NodeKey, QuadKey, VertKey};
use elements::{Quad, Vert};

struct LeafNode {
    verts: HashMap<VertKey, Vert>,
    quads: HashMap<QuadKey, Quad>,
}

impl LeafNode {
    fn new() -> Self {
        Self {
            verts: HashMap::new(),
            quads: HashMap::new(),
        }
    }
}

struct InternalNode {
    children: [NodeKey; 2],
}

impl InternalNode {
    fn new(children: [NodeKey; 2]) -> Self {
    }
}

enum Node {
    Leaf(LeafNode),
    Internal(InternalNode),
}

pub struct Bvh {
    root: NodeKey,
    nodes: HashMap<NodeKey, Node>,
    key_maker: KeyMaker,
}

impl Bvh {
    pub fn new() -> Self {
        let mut key_maker = KeyMaker::new();
        let nk = key_maker.node();
        let mut nodes = HashMap::new();
        let node = Node::Leaf(LeafNode::new());
        nodes.insert(nk, node);
        Self { root: nk, nodes: nodes, key_maker: key_maker }
    }

    pub fn split_leaf(&mut self, leaf: NodeKey) {
        let node = Node::Internal(InternalNode::new());
    }
}
