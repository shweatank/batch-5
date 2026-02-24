Patient :: Patient() {
        cout << "Enter the name of Patient: " << endl;
        cin >> name;
        cout << "Enter the health status of Patient: " << endl;
        cin >> healthStatus;
        cout << "Enter the age of Patient: " << endl;
        cin >> age;
	cout << "Enter the phone number of Patient : " << endl;
	cin >> phoneno;
}

void Patient :: print() {
        cout << "patient name : " << name << endl;
        cout << "patient health status : " << healthStatus << endl;
        cout << "patient age : " << age << endl;
	cout << "patient phoneno : " << phoneno << endl;
}

//void Patient :: health_status() {
        //cout << name << " Patient's updated health status: " << healthStatus << endl;    
//}

