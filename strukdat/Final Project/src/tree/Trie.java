package tree;

import java.util.ArrayList;
import java.util.List;

/**
 * Trie (Prefix Tree) for fast station name lookup and autocomplete.
 * Implemented from scratch using OOP — no library.
 *
 * Each TrieNode holds:
 * - children[]: array of 128 ASCII slots (handles a-z, A-Z, space, numbers)
 * - isEnd: marks end of a complete station name
 * - stationId: the id of the station at this terminal node
 * - character: for debugging/display
 */
public class Trie {

    private static class TrieNode {
        TrieNode[] children;
        boolean isEnd;
        int stationId;
        char ch;

        TrieNode(char ch) {
            this.ch = ch;
            this.children = new TrieNode[128];
            this.isEnd = false;
            this.stationId = -1;
        }
    }

    private TrieNode root;
    private int count;

    public Trie() {
        root = new TrieNode('\0');
        count = 0;
    }

    /**
     * Insert a station name with its id.
     * Case-insensitive — stored as lowercase.
     */
    public void insert(String name, int stationId) {
        if (name == null || name.isEmpty())
            return;
        String lower = name.toLowerCase();
        TrieNode curr = root;

        for (int i = 0; i < lower.length(); i++) {
            char c = lower.charAt(i);
            if (c >= 128)
                continue; // skip non-ASCII
            if (curr.children[c] == null) {
                curr.children[c] = new TrieNode(c);
            }
            curr = curr.children[c];
        }

        if (!curr.isEnd)
            count++;
        curr.isEnd = true;
        curr.stationId = stationId;
    }

    /**
     * Exact search — returns stationId or -1 if not found.
     */
    public int search(String name) {
        TrieNode node = findNode(name);
        if (node == null || !node.isEnd)
            return -1;
        return node.stationId;
    }

    /**
     * Prefix search — returns list of all station ids whose name starts with
     * prefix.
     */
    public List<Integer> searchByPrefix(String prefix) {
        List<Integer> results = new ArrayList<>();
        TrieNode node = findNode(prefix);
        if (node == null)
            return results;
        collectAll(node, results);
        return results;
    }

    /**
     * Returns true if any station name starts with the given prefix.
     */
    public boolean startsWith(String prefix) {
        return findNode(prefix) != null;
    }

    /**
     * Remove a station name from the trie.
     */
    public boolean delete(String name) {
        if (search(name) == -1)
            return false;
        deleteHelper(root, name.toLowerCase(), 0);
        count--;
        return true;
    }

    private boolean deleteHelper(TrieNode node, String name, int depth) {
        if (node == null)
            return false;
        if (depth == name.length()) {
            if (!node.isEnd)
                return false;
            node.isEnd = false;
            return isEmpty(node);
        }
        char c = name.charAt(depth);
        if (c >= 128)
            return false;
        boolean shouldDelete = deleteHelper(node.children[c], name, depth + 1);
        if (shouldDelete) {
            node.children[c] = null;
            return !node.isEnd && isEmpty(node);
        }
        return false;
    }

    private boolean isEmpty(TrieNode node) {
        for (TrieNode child : node.children) {
            if (child != null)
                return false;
        }
        return true;
    }

    private TrieNode findNode(String prefix) {
        if (prefix == null)
            return null;
        String lower = prefix.toLowerCase();
        TrieNode curr = root;
        for (int i = 0; i < lower.length(); i++) {
            char c = lower.charAt(i);
            if (c >= 128)
                return null;
            if (curr.children[c] == null)
                return null;
            curr = curr.children[c];
        }
        return curr;
    }

    private void collectAll(TrieNode node, List<Integer> results) {
        if (node == null)
            return;
        if (node.isEnd)
            results.add(node.stationId);
        for (TrieNode child : node.children) {
            if (child != null)
                collectAll(child, results);
        }
    }

    public int getCount() {
        return count;
    }
}
