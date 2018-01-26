#include <iostream>
#include <Python.h>
using namespace std;


int main (int argc, char *const argv[])
{
	PyObject *pName, *pModule;	//for load python script
	PyObject *pHelloworld, *pInitial, *pgoogle_stt;	//for python functions
	PyObject *pArgs, *pValue, *g_pArgs, *g_pValue;	//for get/set python function parameters

	Py_Initialize();

	//start set path
	PyObject *sys = PyImport_ImportModule("sys");
	PyObject *path = PyObject_GetAttrString(sys, "path");
	PyList_Append(path, PyString_FromString("."));

	//get python script
	pName = PyString_FromString("stt");

	pModule = PyImport_Import(pName);
	Py_DECREF(pName);
	
	if (pModule != NULL)
	{
		//module load
		pHelloworld = PyObject_GetAttrString(pModule, "helloworld");
		pInitial = PyObject_GetAttrString(pModule, "initialize");
		pgoogle_stt = PyObject_GetAttrString(pModule, "google_stt");	
	
		//module load error check
		if ( !(pHelloworld && PyCallable_Check(pHelloworld)) )
		{
			if (PyErr_Occurred()) PyErr_Print();
			std::cout << "Cannot find function 'helloworld'" << std::endl;
			return 1;
		}
		if ( !(pInitial && PyCallable_Check(pInitial)) )
		{
			if (PyErr_Occurred()) PyErr_Print();
			std::cout << "Cannot find function 'initialize'" << std::endl;
			return 1;
		}
        if ( !(pgoogle_stt && PyCallable_Check(pgoogle_stt)) )
        {
            if (PyErr_Occurred()) PyErr_Print();
            std::cout << "Cannot find function 'google_stt'" << std::endl;
            return 1;
        }
			

		int index = 0;
		double lfValue = 0.0;
		string google_string;

		//pHelloworld : function without input/output
		PyObject_CallObject(pHelloworld, NULL);	//should be "Hello world!"

		//pgoogle_stt : function without input/output
        g_pArgs = PyTuple_New(100); //make parameter list
		g_pArgs = PyObject_CallObject(pgoogle_stt, NULL);
		google_string = PyString_AsString(g_pArgs);

		//pInitial : function with single input, single output
		for(int i=0;i<=3;++i)
		{
			pArgs = PyTuple_New(1);	//make parameter list
			pValue = PyInt_FromLong(i);	//input parameter
			PyTuple_SetItem(pArgs, 0, pValue);	//set pArgs with pValue
			pValue = PyObject_CallObject(pInitial, pArgs);	//get result
			
			//If there is return, set value into variables
			if (pValue != NULL)
			{	
				switch(i)
				{
					case 0:
						index = PyInt_AsLong(pValue);
						std::cout << "index: " << index << std::endl;
					break;
					case 1:
						lfValue = PyFloat_AsDouble(pValue);
						std::cout << "value: " << lfValue << std::endl;
					break;
                    case 2:
                        lfValue = PyFloat_AsDouble(pValue);
                        std::cout << "value2: " << lfValue << std::endl;
                    break;
                    case 3:
                        lfValue = PyFloat_AsDouble(pValue);
                        std::cout << "value3: " << lfValue << std::endl;
                    break;

				}
				Py_DECREF(pValue);
			}
			else
			{
				PyErr_Print();
				std::cout << "Call failed" << std::endl;
			}
		}
	}
	else
	{
		PyErr_Print();
		std::cout << "Failed to load 'hello'" << std::endl;
		return 1;
	}
	
	Py_Finalize();
	return 0;
}
