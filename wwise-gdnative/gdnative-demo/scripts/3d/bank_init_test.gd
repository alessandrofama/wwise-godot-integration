extends Node

func _enter_tree():	
	var initBankResult = Wwise.load_bank_id(AK.BANKS.INIT)
	print("Init Bank result: ", initBankResult)
	
	var testBankResult = Wwise.load_bank_id(AK.BANKS.TESTBANK)
	print("Test Bank result: ", testBankResult)
	
func _exit_tree():
	var unloadTestBankResult = Wwise.unload_bank_id(AK.BANKS.TESTBANK)
	print("Unload Test Bank result: ", unloadTestBankResult)
	
	var unloadInitBankResult = Wwise.unload_bank_id(AK.BANKS.INIT)
	print("Unload Init Bank result: ", unloadInitBankResult)
	
