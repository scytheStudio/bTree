#include<iostream>
#include <vector>
#include <fstream>
#include <windows.h>
using namespace std;

class Node
{
    int n;
    int t;
    bool leaf;
    vector <int> keys;
    vector <Node *> childs;
public:
    Node(int _t, bool _leaf);

    void insertNonFull(int k);
    void splitChild(int i, Node *y);
    void traverse();
    Node *search(int k);

friend class Tree;
};

class Tree
{
    Node *root;
    int t;
public:
    Tree(int _t) {
        root = NULL;  t = _t;
    }

    void traverse() {
        if (root != NULL) root->traverse();
    }

    Node* search(int k) {
        return (root == NULL)? NULL : root->search(k);
    }

    Node* getRoot() {
        return root;
    }

    int minimum(Node *x);
    int maximum(Node *x);

    void insert(int k);

friend class Node;
};

Node::Node(int t1, bool leaf1) {
    t = t1;
    leaf = leaf1;

    keys.reserve(2*t-1);
    childs.reserve(2*t);
    n = 0;
}

void Node::traverse() {


    int i;
    for (i = 0; i < n; i++)
    {
        if (leaf == false)
            childs[i]->traverse();

        cout << " " << keys[i];
    }

    if (leaf == false)
        childs[i]->traverse();

}

Node *Node::search(int k) {
    //Znajdowanie pierwszego większego lub równego lcizbie k klucza
    int i = 0;
    while (i < n && k > keys[i])
        i++;

    //Jeżeli obecny klucz okaże się poszukiwanym to zostanie on zwrócony
    if (keys[i] == k)
        return this;

    //Jeżeli to liść, a poszukiwany klucz nie został odnaleziony to go nie ma w strukturze
    if (leaf == true)
        return NULL;

    //szukamy po dzieciakach
    return childs[i]->search(k);
}

void Tree::insert(int k) {


    if (root == NULL) {

        root = new Node(t, true);
        root->keys[0] = k;
        root->n = 1;
    } else  {
        if (root->n == 2*t-1)
        {
            Node *s = new Node(t, false);

            s->childs[0] = root;

            s->splitChild(0, root);

            int i = 0;
            if (s->keys[0] < k)
                i++;
            s->childs[i]->insertNonFull(k);

            // Change root
            root = s;
        }
        else  // If root is not full, call insertNonFull for root
            root->insertNonFull(k);
    }
}

int Tree::minimum( Node *x) {
    return (x->leaf==true) ? x->keys[0] : minimum(x->childs[0]);
}

int Tree::maximum(Node *x) {
    return (x->leaf==true) ? x->keys[x->n-1] : maximum(x->childs[x->n]);
}

void Node::insertNonFull(int k) {
    int i = n-1;//index pierwszego z prawej elementu

    if (leaf == true) {
        // Ta petla robi miejsce dla nowego klucza
        while (i >= 0 && keys[i] > k)
        {
            keys[i+1] = keys[i];
            i--;
        }
        //dodanie klucza
        keys[i+1] = k;
        n = n+1;
    }
    else {
        // Ta funkcja wybiera dziecko, któremu wstawi nowy element
        while (i >= 0 && keys[i] > k)
            i--;

        if (childs[i+1]->n == 2*t-1) //gdy dziecko jest pełne
        {
            splitChild(i+1, childs[i+1]); //dochodzi do podziału

            if (keys[i+1] < k)
                i++;
        }
        childs[i+1]->insertNonFull(k); //rekurencja w przypadku, gdy aktualny węzeł nie jest
    }
}

void Node::splitChild(int i, Node *y)
{
    // Tworzymy nowy węzeł do trzymania t-1 kluczy
    Node *z = new Node(y->t, y->leaf);
    z->n = t - 1;

    // kopiujemy ostatnią połowę kluczy z y do z
    for (int j = 0; j < t-1; j++)
        z->keys[j] = y->keys[j+t];

    // jeśli węzeł nie jest liściem to kopiujemy ostatnią połowę jego dzieciaków
    if (y->leaf == false)
    {
        for (int j = 0; j < t; j++)
            z->childs[j] = y->childs[j+t];
    }

    y->n = t - 1; //w y jest teraz t-1 kluczy, bo było 2t-1, a usunelismy t

    //Robimy miejsce dla nowego dziecka
    for (int j = n; j >= i+1; j--)
        childs[j+1] = childs[j];

    //Tym dzieckiem będzie z
    childs[i+1] = z;

    //robimy miejsce dla nowego klucza
    for (int j = n-1; j >= i; j--)
        keys[j+1] = keys[j];

    //środkowy klucz zostanie przepisany do węzła
    keys[i] = y->keys[t-1];

    n = n + 1;
}

int main()
{
    Tree t(3);


    fstream inputFile;
    inputFile.open("data.txt", ios::in);


    int choice = 1;	//zmienna wyboru
    int value = 0; 	// zmienna wartosci

    while (choice != 0) {
        system("cls");
        if (t.getRoot()==NULL) cout << "\nAny elements";
        else {
            t.traverse();
        }

        cout << "\n\nHey buddy";

        cout << "\n1. Add node.";
        cout << "\n3. Load data from file.";
        cout << "\n4. Minimum.";
        cout << "\n5. Maximum.";
        cout << "\n6. Does key exist?";
        cout << "\n0. Exit.\n";

        cin >> choice;

        switch (choice)
        {
            case 0:
                cout << "\n\nThe End.\n";
                inputFile.close();
                system("PAUSE");
                return 0;
                break;
            case 1:
                cout << "\nType value ";
                cin >> value;
                t.insert(value);
                break;


            case 3:
                while (inputFile >> value) t.insert(value);
                cout << "\nData loaded";
                Sleep(2000);
                break;

            case 4:
                cout << "Minimum value is: " << t.minimum(t.getRoot()) << endl;
                Sleep(2000);
                break;

            case 5:
                cout << "Maximum value is: " << t.maximum(t.getRoot()) << endl;
                Sleep(2000);
                system("pause");
                break;

            case 6:
                cout << "\nType key to find ";
                cin >> value;
                (t.search(value)!=NULL) ? cout << "Exist\n" : cout << "No exist\n";
                break;

            default:
                cout << "\n\nWrong choice ";
                Sleep(1000);
                break;
        }
    }








    t.insert(10);
    t.insert(20);
    t.insert(5);
    t.insert(6);
    t.insert(12);
    t.insert(30);
    t.insert(7);
    t.insert(17);

    cout << "Traversal of the constucted tree is ";
    t.traverse();

    int k = 6;
    (t.search(k) != NULL)? cout << "\nPresent" : cout << "\nNot Present";

    k = 15;
    (t.search(k) != NULL)? cout << "\nPresent" : cout << "\nNot Present";

    return 0;
}
