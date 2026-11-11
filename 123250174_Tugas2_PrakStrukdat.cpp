#include <iostream>
#include <string>

using namespace std;

struct Menu {
    string name;
    string category;

    float price;
    bool status = true;
};

struct Node {
    Menu m;

    Node* left;
    Node* right;
};


struct Order{
    string customerName;
    string menuName;
};

struct QNode {
    Order o;

    QNode* next;
};

struct LinkedQueue {
    QNode* front = nullptr;
    QNode* rear = nullptr;
};

bool insertMenu(Node*& node, Menu m){
    // BINARY SEARCH TREE
    if (node == nullptr)
    {
        node = new Node{m, nullptr, nullptr};

        return false;
    }
    
    if (m.name < node->m.name){
        return insertMenu(node->left, m);
    } else if (m.name > node->m.name) {
        return insertMenu(node->right, m);
    }

    return true;
}

void showMenu(Node* node){
    // INORDER TRAVERSAL
    if (node == nullptr)
    {
        return;
    }
    
    showMenu(node->left);   
    cout << "Nama Menu: " << node->m.name << endl;
    cout << "Kategori: " << node->m.category << endl;
    cout << "Harga: " << node->m.price << endl;
    cout << "Status: " << (node->m.status ? "Tersedia" : "Habis") << endl;
        
    showMenu(node->right);
}

Node* searchMenu(Node* node, string target){
    if (node == nullptr || target == node->m.name)
    {
        return node;
    }
    
    if (target < node->m.name)
    {
        return searchMenu(node->left, target);
    }
    
    return searchMenu(node->right, target);
}

bool addOrder(LinkedQueue& q, Order o){
   QNode* newQNode = new QNode{o, nullptr};

    if (q.rear == nullptr) q.front = q.rear = newQNode;
    else {
        q.rear->next = newQNode;
        q.rear = newQNode;
    }

    return true;
}

bool dequeueMenu(LinkedQueue& q, const string& menuName, Order& result) {
    QNode* prev = nullptr;
    QNode* cur = q.front;

    while (cur != nullptr && cur->o.menuName != menuName) {
        prev = cur;
        cur = cur->next;
    }
    if (cur == nullptr) return false;

    result = cur->o;

    if (prev == nullptr) q.front = cur->next;
    else prev->next = cur->next;

    if (cur == q.rear) q.rear = prev;

    delete cur;
    return true;
}

void showQueue(const LinkedQueue& q) {
    if (q.front == nullptr) {
        cout << "Antrian kosong\n";
        return;
    }
    
    QNode* temp = q.front;
    while (temp != nullptr) {
        cout << temp->o.customerName << " menunggu menu "
             << temp->o.menuName << endl;
        temp = temp->next;
    }
}

//STACK
struct SNode {
    string type;
    string menu;   
    string customerName;
    SNode* next;
};

Node* deleteMenu(Node*& node, const string& menuName) {
    if (node == nullptr) {
        return nullptr;
    }

    if (menuName < node->m.name) {
        node->left = deleteMenu(node->left, menuName);
    } else if (menuName > node->m.name) {
        node->right = deleteMenu(node->right, menuName);
    } else {
        if (node->left == nullptr) {
            Node* rightChild = node->right;
            delete node;
            return rightChild;
        }
        if (node->right == nullptr) {
            Node* leftChild = node->left;
            delete node;
            return leftChild;
        }

        Node* successor = node->right;
        while (successor->left != nullptr) {
            successor = successor->left;
        }
        node->m = successor->m;
        node->right = deleteMenu(node->right, successor->m.name);
    }

    return node;
}

bool removeMenu(Node*& root, const string& menuName) {
    if (searchMenu(root, menuName) == nullptr) {
        return false;
    }
    root = deleteMenu(root, menuName);
    return true;
}

struct Stack {
    SNode* top = nullptr;
};

void push(Stack& s, string type, string menu, string customerName) {
    SNode* newSNode = new SNode{type, menu, customerName, s.top};
    s.top = newSNode;
}

bool pop(Stack& s, SNode& hasil) {
    if (s.top == nullptr) return false;
    SNode* temp = s.top;
    hasil = *temp;
    s.top = temp->next;
    delete temp;
    return true;
}

int main() {
    Node* root = nullptr;
    LinkedQueue queue;
    Stack undoStack;
    int option = -1;

    while (option != 0)
    {
        cout << "=============================" << endl;
        cout << "      RESTORAN KELUARGA      " << endl;
        cout << "=============================" << endl;
        cout << "0. Keluar" << endl;
        cout << "1. Masukkan data menu" << endl;
        cout << "2. Tampilkan data menu" << endl;
        cout << "3. Pesan menu" << endl;
        cout << "4. Tandai menu tersedia" << endl;
        cout << "5. Hapus menu" << endl;
        cout << "6. Undo aksi terakhir" << endl;
        cout << "7. Tampilkan waiting list" << endl;
        cout << "8. Tandai menu habis" << endl;
        
        cout << "=============================" << endl;
        cout << "Input: "; cin >> option;
        
        switch (option)
        {
            case 1:
            {
                Menu menu;
                cin.ignore();
                cout << "Masukkan nama menu: "; getline(cin, menu.name);
                cout << "Masukkan kategori menu: "; getline(cin, menu.category);
                cout << "Masukkan harga menu: "; cin >> menu.price;
                if (insertMenu(root, menu)) {
                    cout << "Menu berhasil ditambahkan.\n";
                } else {
                    cout << "Nama menu sudah terdaftar.\n";
                }
                break;
            }
            case 2:
                showMenu(root);
                break;
            case 3:
            {
                Order order;
                cin.ignore();
                cout << "Nama pelanggan: "; getline(cin, order.customerName);
                cout << "Nama menu: "; getline(cin, order.menuName);
                {
                    Node* temp = searchMenu(root, order.menuName);
                    if (temp == nullptr) {
                        cout << "Menu tidak ditemukan!\n";
                    } else if (temp->m.status) {
                        cout << "Pesanan " << order.customerName
                             << " berhasil dibuat\n";
                        push(undoStack, "pesan", order.menuName,
                             order.customerName);
                    } else {
                        cout << "Menu sedang habis, " << order.customerName
                             << " masuk ke dalam waiting list\n";
                        addOrder(queue, order);
                    }
                }
                break;
            }
            case 4:
            {
                Menu menu;
                cin.ignore();
                cout << "Nama menu: "; getline(cin, menu.name);
                {
                    Node* temp = searchMenu(root, menu.name);
                    if (temp == nullptr) {
                        cout << "Menu tidak ditemukan!\n";
                    } else if (temp->m.status) {
                        cout << "Menu masih tersedia!\n";
                    } else {
                        temp->m.status = 1;
                        cout << "Menu tersedia kembali\n";
                        push(undoStack, "update", menu.name, "");

                        Order servedOrder;
                        if (dequeueMenu(queue, menu.name, servedOrder)) {
                            cout << "Pesanan langsung diberikan kepada "
                                 << servedOrder.customerName
                                 << " dari waiting list\n";
                        }
                    }
                }
                break;
            }
            case 5:
            {
                Menu menu;
                cin.ignore();
                cout << "Nama menu: "; getline(cin, menu.name);
                if (removeMenu(root, menu.name))
                    cout << "Menu berhasil dihapus\n";
                else
                    cout << "Menu tidak ditemukan!\n";
                break;
            }
            case 6:
                {
                    SNode action;
                    if (!pop(undoStack, action)) {
                        cout << "Tidak ada aksi untuk di-undo\n";
                    } else if (action.type == "pesan") {
                        cout << "Undo: Pesanan menu " << action.menu << " dibatalkan\n";
                    } else {
                        Node* temp = searchMenu(root, action.menu);
                        if (temp != nullptr)
                            temp->m.status = 0;
                        cout << "Undo: Menu " << action.menu << " kembali habis\n";
                    }
                }
                break;
            case 7:
                showQueue(queue);
                break;
            case 8:
            {
                Menu menu;
                cin.ignore();
                cout << "Nama menu: "; getline(cin, menu.name);
                {
                    Node* temp = searchMenu(root, menu.name);
                    if (temp == nullptr) {
                        cout << "Menu tidak ditemukan!\n";
                    } else {
                        temp->m.status = 0;
                        cout << "Menu " << menu.name << " ditandai habis\n";
                    }
                }
                break;
            }

            case 0:
                cout << "Terima Kasih!.\n";
                break;

            default:
                cout << "Pilihan tidak valid.\n";
        }
    }

    return 0;
}