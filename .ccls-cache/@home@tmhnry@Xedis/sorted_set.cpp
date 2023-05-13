#include "common.h"
#include "zset.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

static ZNode *znode_new(const char *name, size_t len, double score) {
  ZNode *node = (ZNode *)malloc(sizeof(ZNode) + len);
  assert(node);
  avl_init(&node->tree);
  node->hmap.next = NULL;
  node->hmap.hcode = str_hash((uint8_t *)name, len);
  node->score = score;
  node->len = len;
  memcpy(&node->name[0], name, len);
  return node;
}

// insert into the AVL tree
static void tree_add(ZSet *zset, ZNode *node) {
  if (!zset->tree) {
    zset->tree = &node->tree;
    return;
  }

  AVLNode *cur = zset->tree;
  while (true) {
    AVLNode **from = zless(&node->tree, cur) ? &cur->left : &cur->right;
    if (!*from) {
      *from = &node->tree;
      node->tree.parent = cur;
      zset->tree = avl_fix(&node->tree);
      break;
    }
    cur = *from;
  }
}

// compare by the (score, name) tuple
static bool zless(AVLNode *lhs, double score, const char *name, size_t len) {
  ZNode *zl = container_of(lhs, ZNode, tree);
  if (zl->score != score) {
    return zl->score < score;
  }
  int rv = memcmp(zl->name, name, min(zl->len, len));
  if (rv != 0) {
    return rv < 0;
  }
  return zl->len < len;
}

static bool zless(AVLNode *lhs, AVLNode *rhs) {
  ZNode *zr = container_of(rhs, ZNode, tree);
  return zless(lhs, zr->score, zr->name, zr->len);
}

// update the score of an existing node (AVL tree reinsertion)
static void zset_update(ZSet *zet, ZNode *node, double score) {
  if (node->score == score) {
    return;
  }
  zset->tree = avl_del(&node->tree);
  node->score = score;
  avl_init(&node->tree);
  tree_add(zset, node);
}

// add a new (score, name) tuple, or update the score of the existing tuple
bool zset_add(ZSet *zset, const char *name, size_t len, double score) {
  ZNode *node = zset_lookup(zset, name, len);
  if (node) {
    zset_update(zset, node, score);
    return false;
  } else {
    node = znode_new(name, len, score);
    hm_insert(&zset->hmap, &node->hmap);
    tree_add(zset, node);
    return true;
  }
}

ZNode *zset_query(ZSet *zset, double score, const char *name, size_t len,
                  int64_t offset) {
  AVLNode *found = NULL;
  AVLNode *cur = zset->tree;
  while (cur) {
    if (zless(cur, score, name, len)) {
      cur = cur->right;
    } else {
      found = cur;
      cur = cur->left;
    }
  }

  if (found) {
    found = avl_offset(found, offset);
  }
  return found ? container_of(found, ZNode, tree) : NULL;
}
