#include "mesh_instance_2d.h"

void MeshInstance2D::on_tree_entered() {
    _sync_server_node_transform();
}

void MeshInstance2D::on_server_node_transform_changed() {
    _sync_server_node_transform();
}