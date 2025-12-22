/*
 * NonExistentStub.c
 *
 * Stub library that provides nonExistentFunction.
 * This library is built and then deleted to create a missing dependency
 * for TestLibraryMissingDeps.
 */

#ifdef _WIN32
__declspec(dllexport)
#endif
int nonExistentFunction()
{
	return 0;
}
