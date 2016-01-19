#include <Python.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#define MIN(x, y) ((x) > (y) ? (y) : (x))
#define ISALPHANUM(x)	(isalpha(x) || isdigit(x))
static inline int getWeight(char c) 
{
	return (ISALPHANUM(c) ? 10 : 1);
}

static inline int getSubWeight(char c, char d) 
{
	return (ISALPHANUM(c) || ISALPHANUM(d)) ? 10 : 1;
}

static PyObject* py_weighted_levenshtein(PyObject* self, PyObject* args)
{
	char *a;
	char *b;
	int rc;
	int ret = 0;
	int i, j;
	rc = PyArg_ParseTuple(args, "ss", &a, &b);
	int n = strlen(a);
	int m = strlen(b);
	int **d = NULL;
	d = (int **)PyMem_Malloc(sizeof(int *) * (n+1));
	//assert(d != NULL);
	
	d[0] = NULL;
	for(i = 0; i < n+1; i++) {
		d[i] = (int *)PyMem_Malloc(sizeof(int) * (m+1));
		memset(d[i], 0, sizeof(int) * (m+1));
	}
	for(i = 1; i < n+1; i++)
		d[i][0] = d[i-1][0] + getWeight(a[i-1]);
	for(i = 1; i < m+1; i++)
		d[0][i] = d[0][i-1] + getWeight(b[i-1]);
	for(j = 1; j < m+1; j++) {
		for(i = 1; i < n+1; i++) {
			int subweight = a[i-1] == b[j-1] ? 0 : getSubWeight(a[i-1], b[j-1]);
			d[i][j] = MIN(d[i-1][j-1] + subweight,
						MIN(d[i-1][j] + getWeight(a[i-1]), 
							d[i][j-1] + getWeight(b[j-1])));
		}
	}
	ret = d[n][m];
	for(i = 0; i < n+1; i++)
		PyMem_Free(d[i]);
	PyMem_Free(d);
	return Py_BuildValue("i", ret);
}

static PyMethodDef methods[] = {
	{"weighted_levenshtein", py_weighted_levenshtein, METH_VARARGS},
	{NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
initwlev(void)
{
	(void) Py_InitModule("wlev", methods);
}
