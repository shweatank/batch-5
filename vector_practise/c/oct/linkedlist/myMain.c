#include "myheader.h"

int main() {
	student *head = NULL;
	char ch;
	while(1) {
		printf("i.insert_at_last f.insert_at_first w.insert_at_sorted_order p.print d.delete_at_first s.save_into_file q.sync_from_file e.exit\n");
		printf("Enter the choice: ");
		__fpurge(stdin);
		scanf("%c",&ch);
		switch(ch) {
			case 'i':
				head = insertAtLast(head);
				break;
			case 'p':
				printStudents(head);
				break;
			case 'f':
				head = insertAtFirst(head);
				break;
			case 'd':
				head = deleteAtFirst(head);
				break;
			case 's':
				saveIntoFile(head);
				break;
			case 'q':
				head = syncFromFile(head);
				break;
			case 'w':
				head = insertAtSortedOrder(head);
				break;
			case 'e':
				exit(0);
		}
	}
}

