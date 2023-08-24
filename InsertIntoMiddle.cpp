#include <iostream>
#include <mutex>
#include <memory>

class FineGrainedQueue {
public:
    FineGrainedQueue() : head(new Node(-1)), queue_mutex(new std::mutex()) {}

    ~FineGrainedQueue() {
        Node* current = head;
        while (current != nullptr) {
            Node* temp = current;
            current = current->next;
            delete temp->node_mutex;
            delete temp;
        }
        delete queue_mutex;
    }

    void insertIntoMiddle(int value, int pos) {
        Node* new_node = new Node(value);

        std::unique_lock<std::mutex> queue_lock(*queue_mutex);

        Node* current = head;

        for (int i = 0; i < pos - 1 && current->next != nullptr; ++i) {
            current = current->next;
        }

        if (pos >= 0) {
            new_node->next = current->next;
            current->next = new_node;
        }
        else {
            while (current->next != nullptr) {
                current = current->next;
            }
            new_node->next = nullptr;
            current->next = new_node;
        }

        queue_lock.unlock();
    }

    void printList() {
        std::unique_lock<std::mutex> queue_lock(*queue_mutex);
        Node* current = head->next;

        while (current != nullptr) {
            std::unique_lock<std::mutex> current_lock(*current->node_mutex);
            std::cout << current->value << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }

private:
    struct Node {
        int value;
        Node* next;
        std::mutex* node_mutex;

        Node(int val) : value(val), next(nullptr), node_mutex(new std::mutex()) {}
    };

    Node* head;
    std::mutex* queue_mutex;
};

int main() {
    FineGrainedQueue queue;
    queue.insertIntoMiddle(2, 0);
    queue.insertIntoMiddle(3, 1);
    queue.insertIntoMiddle(4, 1);

    queue.printList();

    return 0;
}
