#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
using namespace std;

struct ProductData {
    int ID = 0;
    string name;
    float price = 0;
    string category;
};

struct ProductDataNode {
    ProductData* data = nullptr;
    ProductDataNode* next = nullptr;
    ProductDataNode* last = nullptr;
};

static void PrintProductDataNode(ProductDataNode* node, int num) {
    ProductData* data = node->data;
    cout << "\nINDEX: (" << num << ") ID: (" << data->ID << ")      NAME: (" << data->name << ")      PRICE: ($" << data->price << ")      CATEGORY: (" << data->category << ")";
};

class LinkedList {
private:
    int size = 0;
    ProductDataNode* head = nullptr;
    ProductDataNode* tail = nullptr;

    bool IsInsideListRange(int index) const { return (index >= 0 && index < size); }

    void SwapData(ProductDataNode* node1, ProductDataNode* node2) {
        cout << "\nSwapping Price: " << node1->data->price << " and " << node2->data->price;
        ProductData* temp = node1->data;
        node1->data = node2->data;
        node2->data = temp;
    }
public:
    ~LinkedList() {
        for (int i = 0; i < size; i++) {
            PopNode();
        }
    }

    ProductData* GetProductData(int index) {
        if (!IsInsideListRange(index)) {
            return nullptr;
        }

        ProductDataNode* node = GetNode(index);
        return node->data;
    }

    // Insertion Sort
    void Sort() {
        if (head == nullptr) {
            cout << "\nList is Empty";
            return;
        }

        cout << "\nInsertion Sort Initiated";
        cout << "\n--------------------------------------------";
        ProductDataNode* node = head;

        while (node->next != nullptr) {
            if (node->data->price > node->next->data->price) {
                ProductDataNode* temp = node;
                do {
                    SwapData(temp, temp->next);
                    temp = temp->last;
                    if (temp == nullptr) {
                        break;
                    }
                } while (temp->data->price > temp->next->data->price);
            }
            node = node->next;
        }
        cout << "\n--------------------------------------------";
        cout << "\nInsertion Sort Completed";
    }

    ProductData* Search(int ID) {
        int index = 0;
        ProductDataNode* node = head;
        while (node != nullptr)
        {
            if (node->data->ID == ID) {
                cout << "\nFound Element With ID(" << ID << ") At Index (" << index << ")\n";
                return node->data;
            }
            index++;
            node = node->next;
        }
        cout << "\nCould Not Locate Element With ID(" << ID << ")\n";
        return nullptr;
    }

    ProductData* Search(string name) {
        int index = 0;
        ProductDataNode* node = head;
        while (node != nullptr)
        {
            if (node->data->name == name) {
                cout << "\nFound Element With Name(" << name << ") At Index (" << index << ")\n";
                return node->data;
            }
            index++;
            node = node->next;
        }
        cout << "\nCould Not Locate Element With Name(" << name << ")\n";
        return nullptr;
    }
    

    void PushNode(ProductDataNode* node) {
        if (head == nullptr) {
            head = node;
            tail = node;
        }
        else {
            tail->next = node;
            node->last = tail;
            tail = node;
        }
        size++;
    }

    void InsertNode(int index, ProductDataNode* newNode) {
        if (index == size) {
            PushNode(newNode);
            return;
        }

        if (!IsInsideListRange(index)) {
            cout << "\nIndex is Outside List Range (Insert)";
            return;
        }

        ProductDataNode* node = GetNode(index);

        if (node->last != nullptr) {
            node->last->next = newNode;
            newNode->last = node->last;
        }
        else {
            head = newNode;
            newNode->next = node;
        }

        node->last = newNode;
        newNode->next = node;
        size++;
    }

    void PopNode() {
        if (tail == nullptr) {
            return;
        } 
        else {
            if (head == tail) {
                head = nullptr;
                delete tail->data;
                delete tail;
                tail = nullptr;
            } 
            else {
                ProductDataNode* temp = tail->last;
                delete tail->data;
                delete tail;
                tail = temp;
            }
        }
        size--;
    }

    void RemoveNode(int index) {
        if (!IsInsideListRange(index)) {
            cout << "\nIndex is Outside List Range (Remove)";
            return;
        }

        ProductDataNode* node = GetNode(index);

        PrintProductDataNode(node, 0);
        cout << " was removed";


        if (node->next != nullptr) {
            node->next->last = node->last;
        }
        else {
            tail = node->last;
        }

        if (node->last != nullptr) {
            node->last->next = node->next;
        }
        else {
            head = node->next;
        }

        delete node->data;
        delete node;
        size--;
    }

    ProductDataNode* GetNode(int index) {
        ProductDataNode* node = head;
        for (int i = 0; i < index; i++) {
            if (node == nullptr) {
                cout << "\nIndex Out Of Bounds";
                return nullptr;
            }
            else {
                node = node->next;
            }
        }
        return node;
    }

    int Size() const {
        return size;
    }

    void PrintList() {
        if (head == nullptr) {
            return;
        }

        cout << "\n";
        int num = 0;
        ProductDataNode* node = head;
        while (node != nullptr)
        {
            PrintProductDataNode(node, num);
            node = node->next;
            num++;
        }
        cout << "\n";
    }
};

static void ReadDataIntoLinkedList(string path, LinkedList* linkedList) {
    int num = 0;
    string line;
    ifstream dataFile(path);
    if (dataFile.is_open()) {
        while (getline(dataFile, line)) {

            string delimiter = ", ";
            size_t pos = 0;
            string token;

            ProductDataNode* dataNode = new ProductDataNode;
            ProductData* data = new ProductData;

            token = line.substr(0, line.find(delimiter));
            line.erase(0, token.length() + delimiter.length());
            data->ID = stoi(token);

            token = line.substr(0, line.find(delimiter));
            line.erase(0, token.length() + delimiter.length());
            data->name = token;

            token = line.substr(0, line.find(delimiter));
            line.erase(0, token.length() + delimiter.length());
            data->price = stof(token);

            token = line.substr(0, line.find(delimiter));
            line.erase(0, token.length() + delimiter.length());
            data->category = token;

            dataNode->data = data;

            linkedList->PushNode(dataNode);
            PrintProductDataNode(dataNode, num);
            cout << " was read from file";
            num++;
        }
    }
};

int main()
{
    LinkedList linkedList;
    // Load Data
    cout << "-------------------------------------------------------------\n(Load Data)\n";
    ReadDataIntoLinkedList("Data Files\\product_data.txt", &linkedList);

    // Insert
    cout << "\n\n-------------------------------------------------------------\n(Insert)\n";
    ProductDataNode* dataNode = new ProductDataNode;
    ProductData* data = new ProductData;
    data->ID = 11111;
    data->name = "Watermelon";
    data->price = 200.12;
    data->category = "Misc";
    dataNode->data = data;
    linkedList.InsertNode(25, dataNode);
    linkedList.PrintList();
    
    // Update
    cout << "\n\n-------------------------------------------------------------\n(Update)\n";
    PrintProductDataNode(dataNode, 0);
    ProductData* productData = linkedList.Search("Watermelon");
    productData->name = "Melon of Water";
    PrintProductDataNode(dataNode, 0);
    productData = linkedList.GetNode(25)->data;
    productData->price = 999.99;
    PrintProductDataNode(dataNode, 0);

    // Delete
    cout << "\n\n-------------------------------------------------------------\n(Delete)\n";
    linkedList.PrintList();
    linkedList.RemoveNode(2);
    linkedList.PrintList();

    // Search
    cout << "\n\n-------------------------------------------------------------\n(Search)\n";
    linkedList.Search("Smartphone ILGCU");
    linkedList.Search(18086);

    //Sort
    cout << "\n\n-------------------------------------------------------------\n(Sort)\n";
    linkedList.PrintList();
    using namespace std::chrono;
    auto startTime = system_clock::now();
    linkedList.Sort();
    auto endTime = system_clock::now();
    auto dif = (duration_cast<milliseconds>(endTime - startTime));
    cout << "\n(The insertion sort took around " << dif.count() << " milliseconds)\n";
    linkedList.PrintList();

    return 0;
}