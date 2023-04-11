#pragma warning (disable: 4996)
#include <iostream>
#include <fstream>
#include <Windows.h>
using namespace std;

struct Student {							// однонаправленный список
	char name[256];
	Student* ptrNext;
};

Student* head = NULL;
int n = 0;
bool flag1 = false;							// проверка, создан ли список или нет

Student* create(const int);						// Создание однонаправленного списка
void deleteNode(const int, int&);					// Удаление элемента
void insertDoubles(const int, int);					// Добавление элементов по два
void insertNode(const int, int&);					// Добавление элемента
void fill();								// заполнение списка ключами (вводится с клавиатуры)
void print();								// вывод списка на консоль
void writeToFile(FILE*);						// запись в файл
void destroy();								// уничтожение списка
void restoreFromFile(FILE*);						// восстановление из файла
void menu(const int);							// непосредственно меню

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);	
	while (true) {
		system("pause");
		system("cls");
		cout << "Здравствуйте, что вы хотите сделать?" << endl;
		cout << "1. Создать список" << endl;
		cout << "2. Добавить по K элементов к элементу на позиции pos слева и справа" << endl;
		cout << "3. Удалить элемент из списка" << endl;
		cout << "4. Напечатать список" << endl;
		cout << "5. Записать список в файл" << endl;
		cout << "6. Уничтожить список" << endl;
		cout << "7. Восстановить список из файла" << endl; cout << endl;
		cout << "Введите c" << endl;
		int c;
		cin >> c;
		system("cls");
		menu(c);
	}
	return 0;
}

Student* create(const int n) {			// первоначальное создание списка вправо
	Student* first = NULL;
	Student* ptr;
	for (int i = n; i > 0; i--) {
		ptr = new(Student);
		ptr->ptrNext = first;
		first = ptr;
	}
	return first;
}

void deleteNode(const int pos, int& n) {
	int c;
	if (pos > n) { cout << "Ошибка 1"; exit(1); }
	if (pos == 1 && pos == n) { c = 0; }
	else if (pos == 1) { c = 1; }
	else if (pos == n) { c = 2; }
	else if (pos > 1 && pos < n) c = 3;
	else exit(2);
	Student* ptr = head;

	switch (c) {
	case 0:
	{
		delete ptr; ptr = NULL;
		head = NULL;
		break;
	}
	case 1:
	{
		head = ptr->ptrNext;
		delete ptr; ptr = NULL;
		break;
	}
	case 2:
	{
		while (ptr->ptrNext->ptrNext != NULL) {
			ptr = ptr->ptrNext;
		}
		Student* ptr1 = ptr->ptrNext;
		delete ptr1; ptr->ptrNext = NULL;
		break;
	}
	case 3:
	{
		for (int i = 1; i < pos - 1; i++) {
			ptr = ptr->ptrNext;
		}
		Student* ptr1 = ptr->ptrNext->ptrNext;
		delete ptr->ptrNext;
		ptr->ptrNext = ptr1;
	}
	}
	n--;
}
void insertDoubles(const int pos, int K) {
	int c = pos;
	int i = 0;
	while (i < K) {
		insertNode(c, n);
		c++;
		i++;
	}
	c += 1;
	i = 0;
	while (i < K) {
		insertNode(c, n);
		c++;
		i++;
	}
}
void insertNode(const int pos, int& n) {				// param - до или после вставить true - до, false - после
	int c;
	if (pos > n) { c = 1; }
	else if (pos == 1) { c = 0; }
	else { c = 2; }									// вставляет на позицию элемента, отодвигая его
	char name[256];
	cout << "Имя?" << endl;
	gets_s(name, 256);
	Student* ptr = head;
	Student* temp = new(Student);
	strcpy_s(temp->name, name);

	switch (c) {
	case 0:
	{
		head = temp;
		temp->ptrNext = ptr;
		break;
	}
	case 1:
	{
		while (ptr->ptrNext != NULL) {				// нахожусь на последнем элементе
			ptr = ptr->ptrNext;
		}
		ptr->ptrNext = temp;
		temp->ptrNext = NULL;
		break;
	}
	case 2:
	{
		int i = 1;
		while (i < pos - 1) {						// нахожусь на элемент раньше нужного
			ptr = ptr->ptrNext;
			i++;
		}
		Student* tempValue = ptr->ptrNext;			// получает ссылку на элемент на место которого вставляем
		ptr->ptrNext = temp;						// ptr теряет ссылку на следующий элемент и получает новую ссылку
		temp->ptrNext = tempValue;					// в веденную структуру записывается ссылка следующего за ней элемента
		if (pos == n) temp->ptrNext->ptrNext = NULL;
		break;
	}
	}
	n++;
}
void fill() {
	cout << "Введите индетификаторы n пользователей: " << endl; cin.ignore();
	for (Student* ptr = head; ptr != NULL;) {
		char nameT[255 + 1];
		cin.getline(nameT, 255);
		strcpy_s(ptr->name, nameT);
		ptr = ptr->ptrNext;
	}
}
void print() {
	cout << "Ваш список: " << endl;
	int counter = 1;
	for (Student* ptr = head; ptr != NULL;) {
		cout << counter++ << " " << ptr->name << endl;
		ptr = ptr->ptrNext;
	}
}

void writeToFile(FILE* file) {
	if ((file = fopen("myFile.txt", "w+")) == NULL) {
		cout << "Ошибка при записи файла" << endl; exit(3);
	}
	Student* ptr = head;
	while (ptr != NULL) {
		fwrite(ptr->name, sizeof(ptr), 1, file);
		ptr = ptr->ptrNext;
	}
	fclose(file);
}
void destroy() {
	Student* ptr = head;
	while (ptr != NULL) {
		Student* ptr1 = ptr;
		ptr = ptr->ptrNext;
		delete ptr1;
	}
	n = 0;
	head = NULL;
}
void restoreFromFile(FILE* file) {
	if ((file = fopen("myFile.txt", "r+")) == NULL) {
		cout << "Ошибка при открытии файла" << endl; exit(4);
	}

	Student* ptr = new(Student);
	fread(ptr->name, sizeof(ptr), 1, file);
	head = ptr;
	Student* last = ptr;
	if (!feof(file)) {
		while (!feof(file)) {
			last = ptr;
			ptr->ptrNext = new(Student);
			ptr = ptr->ptrNext;
			fread(ptr->name, sizeof(ptr), 1, file);
			n++;
		}
		delete last->ptrNext; last->ptrNext = NULL;
	}
	else {
		delete ptr; head = NULL;
	}
	fclose(file);
}
void menu(const int c) {
	FILE* file = NULL;

	if ((c == 1 || c == 7) && flag1) {					// для случая, когда список уже создан
		cout << "Вы уже создали список" << endl;
		return;
	}
	else if (c > 1 && c < 7 && !flag1) {				// для случая, когда список еще не создан
		cout << "Создайте/ восстановите список" << endl;
		return;
	}
	else if (c >= 2 && c <= 5 && n == 0) { cout << "Список пуст." << endl; return; }
	switch (c) {
	case 1:
	{
		flag1 = true;
		cout << "Введите количество структур списка: ";
		cin >> n;
		while (n <= 0) {
			cout << "Нельзя ввести столько структур" << endl;
			cin >> n;
		}
		head = create(n);				// создание и заполнение
		fill();
		break;
	}
	case 2:
	{
		cout << "Введите позицию для добавления и количество элементов для добавления" << endl;
		int pos, K;
		cin >> pos >> K;
		cin.get();									// нормализация буфера для работы без перебоев
		while (pos > n) { cout << "В списке всего " << n << "позиций" << endl; cin >> pos; }
		insertDoubles(pos, K);			// добавление элементов с двух сторон
		print();
		break;
	}
	case 3:
	{
		int pos;
		cout << "Введите позицию для удаления" << endl;
		cin >> pos;
		while (pos > n) { cout << "В списке всего " << n << " позиций" << endl; cin >> pos; }
		deleteNode(pos, n);
		print();
		break;
	}
	case 4:
	{
		print();
		break;
	}
	case 5:
	{
		writeToFile(file);
		break;
	}
	case 6:
	{
		destroy();
		cout << "Список удален" << endl;
		flag1 = false;
		break;
	}
	case 7:
	{
		cout << "Восстановление списка из файла" << endl;
		int n1 = n;
		restoreFromFile(file);
		if (n - n1 == 0) cout << "Нечего восстанавливать" << endl;
		else flag1 = true;
		break;
	}
	default:
	{
		cout << "Выход из меню" << endl;
		destroy();
		exit(777); 
	}
	}
}
