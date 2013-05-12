
#include <assert.h>

#include <jni.h>
#include <git2.h>

#include "util.h"
#include "reference.h"
#include "oid.h"

JNIEXPORT jobject JNICALL
Java_org_libgit2_jagged_core_NativeMethods_referenceResolve(
	JNIEnv *env,
	jclass class,
	jobject repo_java,
	jstring refname_java)
{
	char *refname = NULL;
	git_reference *symbolic = NULL, *resolved = NULL;
	git_repository *repo;
	int error = 0;
	jobject ref_java = NULL;

	assert(env);
	assert(class);
	assert(refname_java);

	if ((refname = git_java_jstring_to_utf8(env, refname_java)) == NULL)
		return NULL;

	if ((repo = git_java_handle_get(env, repo_java)) == NULL)
		goto done;

	if ((error = git_reference_lookup(&symbolic, repo, refname)) < 0 ||
		(error = git_reference_resolve(&resolved, symbolic)) < 0) {
		git_java_exception_throw_from_giterr(env, error);
		goto done;
	}

	ref_java = git_java_reference_init(env, resolved);

done:
	git_reference_free(resolved);
	git_reference_free(symbolic);
	git_java_utf8_free(env, refname_java, refname);

	return ref_java;
}
