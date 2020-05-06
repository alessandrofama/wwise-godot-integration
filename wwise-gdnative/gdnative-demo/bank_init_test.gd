extends Node

func _enter_tree():

	Wwise.set_base_path("Banks/Windows/")
	
	var initBankResult = Wwise.load_bank("Init.bnk")
	print("Init Bank result: ", initBankResult)
	
	var testBankResult = Wwise.load_bank("TestBank.bnk")
	print("Test Bank result: ", testBankResult)
	
	var testErrorResult = Wwise.load_bank("blabla.bnk")
	print("Test Error result: ", testErrorResult)

	
func _exit_tree():
	var unloadTestBankResult = Wwise.unload_bank("TestBank.bnk")
	print("Unload Test Bank result: ", unloadTestBankResult)
	
	var unloadInitBankResult = Wwise.unload_bank("Init.bnk")
	print("Unload Init Bank result: ", unloadInitBankResult)
	
