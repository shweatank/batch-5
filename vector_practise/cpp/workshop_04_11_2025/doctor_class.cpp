Doctor :: Doctor() {
	cout << "Enter the name of doctor: " << endl;
	cin >> name;
	cout << "Enter the specilization of doctor: " << endl;
	cin >> specalization;
	cout << "Enter the emailid of doctor: " << endl;
	cin >> emailid;
}

void Doctor :: print() {
	cout << "doctor name : " << name << endl;
	cout << "doctor specalization : " << specalization << endl;
	cout << "doctor emailid : " << emailid << endl;
}

void Doctor :: updateStatus(Patient &patient) {
	cout << "Enter the new health status of the patient : " << endl;
	cin >> patient.healthStatus;
}

