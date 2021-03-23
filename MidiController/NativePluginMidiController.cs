using System.Runtime.InteropServices;

namespace NativePluginLibrary
{

	public static class MidiController
	{
		const string dllName = "MidiController";

		[DllImport(dllName)]
		public static extern bool OpenMidiController();

		[DllImport(dllName)]
		public static extern bool CloseMidiController();

		[DllImport(dllName)]
		public static extern bool GetSliderValues(float[] values, int first_idx = -1, int last_idx = -1);

		[DllImport(dllName)]
		public static extern bool GetKnobValues(float[] values, int first_idx = -1, int last_idx = -1);

		[DllImport(dllName)]
		public static extern bool GetButtonStates(int[] values, int first_idx = -1, int last_idx = -1);

		[DllImport(dllName)]
		public static extern bool GetSystemButtonStates(int[] values, int first_idx = -1, int last_idx = -1);

		[DllImport(dllName)]
		public static extern float Get1SliderValue(int index);

		[DllImport(dllName)]
		public static extern float Get1KnobValue(int index);

		[DllImport(dllName)]
		public static extern bool Get1ButtonSValue(int index);

		[DllImport(dllName)]
		public static extern bool Get1SystemButtonState(string name);

		[DllImport(dllName)]
		public static extern bool SetButtonOnInterval(int msec);

	}

}