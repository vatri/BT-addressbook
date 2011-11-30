/**
* Address book [console]
* 
* @author Boris Trivic
* @todo read db file from same dir as program-now reads `pwd`
* @todo onDelete(), print message "Deleted" or "User not exists"
* @todo if empty file handle it correctly
* @todo create backup on Save()
* 
**/
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
//#include <string.h>

using namespace std;

/**
* Split string into array (vector) by some separator
* 
* @param string String to split
* @parma string Delimiter
* 
* @return vector
**/
vector<string> strsplit(string str, string separator)
{
	vector<string> res;

	string col = "";
	string chr;
	int i;
	for( i = 0; i < str.size(); i++)
	{
		chr = str[i];
		if( chr != separator)
		{
			col += chr;
		}
		else
		{
			res.push_back(col);
			col = "";
		}
	}
	res.push_back(col);

	return res;
}
/**
* Class for Adddress book manipulation
**/
class Addressbook
{
	public:
		bool file_empty;
		/**
		* Constructor
		**/
		Addressbook()
		{
			DBfile = "data.db";
			
			ifstream fileR(DBfile);
			string tmpLine;
			file_empty = false;
			while( fileR.good() )
			{
				vector<string> arrRow;
			
				getline(fileR, tmpLine);
				if( tmpLine.length() > 0)
				{
					//cout << "FILE OK" << endl;
					arrRow = strsplit(tmpLine, "#");
				}
				else{
					file_empty = true;
				}
				arrData.push_back(arrRow);
			}

			fileR.close();
		}
		/**
		* Display data to user
		**/
//		void DisplayData(bool printData=false)
		void DisplayData()
		{
			if(file_empty)
			{
				cout << "No data" << endl;
				return;
			}
			string rowID;
			string name;
			string add;
			string ph;
			
			string delim = "  |  ";
			cout << "\n";
			cout << "---------------------------" << endl;
			
			cout << "ID" << delim;
			cout << "Name" << delim;
			cout << "Address" << delim;
			cout << "Phone" << delim;
			cout << endl;
			cout << "---------------------------" << endl;
			
			int i;
			//cout << arrData.size();
			//return;
			for(i=0;i< arrData.size();i++)
			{
				rowID = arrData[i][0];
				name = arrData[i][1];
				add = arrData[i][2];
				ph = arrData[i][3];

				string out = rowID+delim+name+delim+add+delim+ph+delim+"\n";//-----------";
				//string cmd = "echo '"+out+"' > tmpFile; lp tmpFile; rm tmpFile";
				//cout << cmd << endl << endl;				
				cout << out;
			}
			cout << "\n\n";
		}
		/**
		* Add new entry
		* 
		* @param string Name
		* @param string Address
		* @param string phone number
		* 
		* @return void
		**/
		void AddRec(string name, string address, string ph)
		{
			string lastID = getNewID();
			vector<string> newRow;

			newRow.push_back(lastID);
			newRow.push_back(name);
			newRow.push_back(address);
			newRow.push_back(ph);
			//cout << "Nr size: " << newRow.size() << endl;
			//arrData.push_back(newRow);
			if( file_empty){
				arrData[0] = newRow;
				file_empty = false;
			}
			else{
				arrData.push_back(newRow);
			}
			cout << "+-----------------+" << endl;
			cout << "| Record inserted |" << endl;
			cout << "+-----------------+" << endl;
		}
		/**
		* Delete entry by ID
		* @todo: dont work if we deleting last entry !
		**/
		void delRec(int ID)
		{
			int i;
			string tmpID;
			for(i=0;i< arrData.size(); i++)
			{
				tmpID = arrData[i][0];
				if( atoi(tmpID.c_str()) == ID )
				{
					arrData.erase(arrData.begin()+i);
				}
			}
			if( arrData.size() < 1){
				//Save(); < memory error: arrData.size()==0 !
				file_empty=true;// because of this Save() not work
				Save();
				cout << "\n\nfile_empty set to true\n";
			}
		}
		/**
		* 
		**/
		bool modifyRec(string newID, string newName, string newAddr, string newPh)
		{
			//@todo: if empty some value, dont modify it
			int recNum = -1;
			for(int i=0; i<arrData.size();i++)
			{
				if( arrData[i][0] == newID)
					recNum = i;
			}
			if( recNum == -1)  return false;
			
			if(newName != "")   arrData[recNum][1] = newName;
			if(newAddr != "")   arrData[recNum][2] = newAddr;
			if(newPh != "")     arrData[recNum][3] = newPh;
			//debug:
			//cout << "recNum=";cout << recNum;cout << endl;
			//string out = "Name: "+newName+ " addr: " + newAddr + " ph: " + newPh;cout << out << endl;
		} 
		/**
		* Check expiring date for person
		* 
		* @param int ID of person
		* 
		* @return void
		**/
		void checkExpire(int id)
		{
			cout << "+---------------------+" << endl;
			cout << "| Expire: 2011-09-10  |" << endl;
			cout << "+---------------------+" << endl;
		}
		/**
		* Save array of data to DB
		**/
		void Save()
		{
			//cout << "Saving:\nbefore" << endl;
			string data = "";
			if(file_empty){
				  //return;
				  data="";
			}
			else
			{
				//cout << "after" << endl;
				// create backup !
				int i;
				for(i=0;i< arrData.size(); i++)
				{
					//data += "\n";
					data += arrData[i][0] + "#";
					data += arrData[i][1] + "#";
					data += arrData[i][2] + "#";
					data += arrData[i][3] + "#";
					if( i < arrData.size()-1)
					{
						data += "\n";
					}
				}
			}
			//cout << "Saving data: " << endl << data << endl;
			ofstream fh(DBfile);
			
			fh << data;
	//		fh << arrData;
			
		}
	private:
		vector<vector<string> > arrData;
		const char * DBfile;
		
		string getNewID()
		{
			if( file_empty)  return "1";
			string maxID;
			int lastIDX = arrData.size()-1;
			maxID = arrData[lastIDX][0];
			int number = atoi( maxID.c_str());
			number++;
			stringstream final;
			final << number;
			//cout << "\nMAX ID=" << final << endl;
			return final.str();
		}
		
};//class


int main(int argc, char *argv[])
{
	const char * clear_cmd = "clear";// linux command for clear console
	//clear_cmd = "";
	#ifdef WIN32
		clear_cmd = "cls";//win
	#endif
	
	bool looping;
	bool cont;//continue automaticaly or ask first
	int opt;
    string msg;
	looping = true;

    Addressbook addr;

	while( looping)
	{
		//system(clear_cmd);
		cont = false;
		
		cout << "+-----------------------------------------+"  << endl;
	    cout << "|                                         |"  << endl;
	    cout << "|   ADDRESSBOOK OPTION PANEL              |"  << endl ;
	    cout << "|                                         |"  << endl;
	    cout << "| 1- Add an address record                |"  << endl;
	    cout << "| 2- Update an address record             |"  << endl;
	    cout << "| 3- Delete an address record             |"  << endl;
	    cout << "| 4- Display all of the address records   |"  << endl;
//		cout << "| 5- Check expire date                    |"  << endl;
	    cout << "| 6- Search record                        |"  << endl;
	    cout << "| 7- Quit                                 |"  << endl;
//	    cout << "| 8- Print all data                       |"  << endl;
	    cout << "|                                         |"  << endl;
	    cout << "+-----------------------------------------+"  << endl;
	    
	    cout << "Enter option number: ";
	    cin >> opt;
	    cin.get();//free object !important
	   
		string name;
		string address;
		string ph;
		string out;
		
		//string modID;//modify ID
	    switch(opt)
	    {
			case 1:
				cout << "Enter name and surname: ";
				getline(cin, name);
				if( name == "")  break;

				cout << "enter address: ";
				getline(cin, address);
				if( address == "") break;
				
				cout << "enter phone number: ";
				getline(cin, ph);
				if(ph == "") break;
				
				//out = name + "#" + address + "#" + ph;
				
				//addr.AddRec(out);
				addr.AddRec(name, address, ph);
				cont = false;
				/**/
			break;
			case 2:
			{
				string modID;
				string newName;
				string newAddr;
				string newPh;
				
				cout << "Enter ID: ";
				getline(cin,modID);
				if( modID == "") break;
				
				cout << "New name:";
				getline(cin, newName);
				
				cout << "New address:";
				getline(cin, newAddr);
				
				cout << "New phone:";
				getline(cin, newPh);
				
				addr.modifyRec(modID, newName, newAddr, newPh);
				
				cont = false;
//				cout << "Modifing record with ID " << modID << endl;
				
			}
			break;
			case 3:
				cout << "Enter ID of row to delete: ";
				int delID;
				cin >> delID;
				addr.delRec(delID);
			break;
			case 4:
				addr.DisplayData();
				cont = false;
			break;
			/*case 5:
				int id;
				cout << "Enter member ID: ";
				cin >> id;
				addr.checkExpire(id);
				cont = false;
			break;*/
			case 7:
				//system("clear");
				system(clear_cmd);
				return 0;
			break;
			/*case 8:
				addr.DisplayData(true);
			break;*/
			
			default:
				msg = "You choosed default value";
				cont = false;
			break;
		}
		//cout << "Saving file..." << endl;
		addr.Save();
		
		//cout << endl << msg << endl;
		if( !cont ){
			cout << "You want to continue using program? (y=yes; n=no)"<<endl;
			//string ans;
			char ans[1];
			cin >> ans;
			if( *ans == 'y')
			{
				continue;
			}
			else
			{
				//system("clear");
				system(clear_cmd);
				return 0;
			}
		}
	}//while()
}
