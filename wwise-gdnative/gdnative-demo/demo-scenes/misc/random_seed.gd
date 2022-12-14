extends Spatial


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


func _ready() -> void:
    
    Wwise.register_listener(self)
    
    Wwise.set_random_seed(12345)
    
    var goResult = Wwise.register_game_obj(self, "TestGO")
    
    print("Registering GameObject: ", goResult)

    var playingID = Wwise.post_event_id(AK.EVENTS.RANDOM_SEED_TEST, self)
