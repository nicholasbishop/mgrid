#[derive(Copy, Clone, Debug, Eq, Hash, PartialEq)]
pub struct NodeKey {
    node: u64,
}

#[derive(Copy, Clone, Debug, Eq, Hash, PartialEq)]
pub struct VertKey {
    node: NodeKey,
    vert: u64,
}

#[derive(Copy, Clone, Debug, Eq, Hash, PartialEq)]
pub struct QuadKey {
    node: NodeKey,
    vert: u64,
}

#[derive(Debug)]
pub struct KeyMaker {
    next_node: u64,
    next_vert: u64,
    next_quad: u64,
}

impl KeyMaker {
    pub fn new() -> Self {
        KeyMaker {
            next_node: 1,
            next_vert: 1,
            next_quad: 1,
        }
    }

    pub fn node(&mut self) -> NodeKey {
        let key = NodeKey { node: self.next_node };
        self.next_node += 1;
        key
    }
}
