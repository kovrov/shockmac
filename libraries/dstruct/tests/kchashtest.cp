/*

Copyright (C) 2015-2018 Night Dive Studios, LLC.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
//=========================================================
//  kcHashTest.cp - Program to test the hash table routines.
//=========================================================

#include <iostream.h>
#include <string.h>

#include "hash.h"

//--------------------------------------
typedef struct						// A struct to place into the hash table
{
	Handle		hdl;
	short			id;
	ResType		type;
	Str255		name;
} kcStruct;


//--------------------------------------
void main(void);
int ResHashFunc(void* v);
int ResEqualFunc(void* v1, void* v2);
bool ResIterFunc(void* elem, void* data);


//--------------------------------------
int ResHashFunc(void* v)
{
	kcStruct	*kc = (kcStruct *)v;
	return (int)((long)kc->type + kc->id);
}

int ResEqualFunc(void* v1, void* v2)
{
	kcStruct	*kc1 = (kcStruct *)v1;
	kcStruct	*kc2 = (kcStruct *)v2;
	if ((kc1->type == kc2->type) && (kc1->id == kc2->id))
   		return 0;
   	else
   		return 1;
}

static bool ResIterFunc(void* elem, void* data)
{
	kcStruct	*kc = (kcStruct *)elem;
	ResType		rt = *(ResType *)data;

	if (kc->type == rt)
	{
		char	name[34];

		cout << kc->hdl << "   ";
		cout.write((const char *)&kc->type, 4);  cout << "   ";
		cout << kc->id << "   '";
		memset(name, 0, 34);
		BlockMove(&kc->name[1], name, kc->name[0]);
		cout << name << "'\n";
	}

	return FALSE;
}

//---------------------------------------------------------
//  Main routine.
//---------------------------------------------------------
void main(void)
{
	StandardFileReply	reply;
	SFTypeList				typeList;
	short						filenum;

	cout << "Select a file - any file...\n\n" ;

	typeList[0] = '????';
	StandardGetFile(nil, -1, typeList, &reply);
	if (!reply.sfGood)
		return;

	filenum = FSpOpenResFile(&reply.sfFile, fsRdPerm);
	if (filenum == -1)
	{
		cout << "Couldn't open the file.\n";
		return;
	}

	// Now, make a hash table of the resources in this file.

	Hashtable	resTable;
	errtype		hasherr;

	hasherr = hash_init(&resTable, sizeof(kcStruct), 2000, ResHashFunc, ResEqualFunc);
	cout << "hash_init result: " << hasherr << endl;

	short			numTypes, numRes;
	short			ti, ri;
	ResType		aResType;
	kcStruct	anElem, *retElem;

	numTypes =  Count1Types();
	for (ti = 1; ti <= numTypes; ti++)
	{
		Get1IndType(&aResType, ti);

		numRes = Count1Resources(aResType);
		SetResLoad(false);
		for (ri = 1; ri <= numRes; ri++)
		{
			anElem.hdl = Get1IndResource(aResType, ri);
			GetResInfo(anElem.hdl, &anElem.id, &anElem.type, anElem.name);

			hasherr = hash_insert(&resTable, &anElem);
			if (hasherr != 0)
				cout << "hash_insert error: " << hasherr << endl;
		}
		SetResLoad(true);
	}

	// Show the state of the hash table after making it.

	cout << "\nFinished making hash table:\n" ;
	cout << "          size: " << resTable.size << endl;
	cout << "      sizelog2: " << resTable.sizelog2 << endl;
	cout << "      elemsize: " << resTable.elemsize << endl;
	cout << "      fullness: " << resTable.fullness << endl;

	// Iterate through the table to show some of the resources.

	cout << "\nIterating through DLOG resources:\n" ;
	aResType = 'DLOG';
   	hash_iter(&resTable, ResIterFunc, &aResType);

	cout << "\nIterating through ALRT resources:\n" ;
	aResType = 'ALRT';
   	hash_iter(&resTable, ResIterFunc, &aResType);

	// Lookup a specific resource.

	cout << "\nLooking up CODE resource 0...   " ;
	anElem.type = 'CODE';
	anElem.id = 0;
	hasherr = hash_lookup(&resTable, &anElem, &retElem);
	if (retElem == NULL)
	{
		cout << "Not found!  Error: " << hasherr << endl;
	}
	else
	{
		char	name[34];

		cout << retElem->hdl << "   ";
		cout.write((const char *)&retElem->type, 4);  cout << "   ";
		cout << retElem->id << "   '";
		memset(name, 0, 34);
		BlockMove(&retElem->name[1], name, retElem->name[0]);
		cout << name << "'\n";
	}

	// Shut her down

	hasherr = hash_destroy(&resTable);
	cout << "\nhash_destroy result: " << hasherr << endl;

	CloseResFile(filenum);
}
