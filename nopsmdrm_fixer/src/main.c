#include <debugScreen.h>
#include <psp2/ctrl.h>
#include <psp2/registrymgr.h>
#include <psp2/kernel/processmgr.h>

#define printf psvDebugScreenPrintf
#define NUL_KEY 0x0000000000000000ll
#define UNI_KEY 0xefcdab8967452301ll

void run()
{
	char key_buf[8];
	uint64_t uni_key = 0xefcdab8967452301LL;

	memset(key_buf, 0, sizeof(key_buf));
	memcpy(&key_buf[0], &uni_key, sizeof(key_buf));
	sceRegMgrSetKeyBin("/CONFIG/NP/", "account_id", key_buf, sizeof(key_buf));

	memset(key_buf, 0, sizeof(key_buf));
	sprintf(key_buf, "%s", "PS Vita");
	sceRegMgrSetKeyStr("/CONFIG/SYSTEM/", "username", key_buf, sizeof(key_buf));

	printf(" Your system has been fixed!\n");
	printf("\nAuto-exit in 5 seconds...\n");
	sceKernelDelayThread(5000000);
	sceKernelExitProcess(0);
}

int main(int argc, char *argv[])
{
	int ret;
	char key_buf[8];
	uint64_t aid_key;
	SceCtrlData ctrl;

	memset(key_buf, 0, sizeof(key_buf));

	psvDebugScreenInit();
	psvDebugScreenClear(0);

	printf("NoPsmDrm Fixer v1.0 by Yoti\n\n");
	printf(" Checking system...\n");

	ret = sceRegMgrGetKeyBin("/CONFIG/NP/", "account_id", key_buf, sizeof(key_buf));
	if (ret < 0)
	{
		printf(" You must enable unsafe homebrew first!\n");
		printf("\nAuto-exit in 5 seconds...\n");
		sceKernelDelayThread(5000000);
		sceKernelExitProcess(0);
	}
	memcpy(&aid_key, key_buf, sizeof(key_buf));

	if (aid_key == UNI_KEY)
	{
		printf(" Your system has already been fixed!\n");
		printf("\nAuto-exit in 5 seconds...\n");
		sceKernelDelayThread(5000000);
		sceKernelExitProcess(0);
	}
	else if (aid_key == NUL_KEY)
	{
		printf(" Your system needs to be fixed!\n");
		printf("\nPress X to fix or O to exit\n\n");

		for(;;)
		{
			sceCtrlPeekBufferPositive(0, &ctrl, 1);
			if (ctrl.buttons == SCE_CTRL_CROSS)
			{
				run();
				break;
			}
			else if (ctrl.buttons == SCE_CTRL_CIRCLE)
			{
				sceKernelExitProcess(0);
				break; // ???
			}
			sceKernelDelayThread(100000);
		}
	}
	else
	{
		printf(" Your system doesn't need to be fixed!\n");
		printf("\nAuto-exit in 5 seconds...\n");
		sceKernelDelayThread(5000000);
		sceKernelExitProcess(0);
	}

	return 0;
}
