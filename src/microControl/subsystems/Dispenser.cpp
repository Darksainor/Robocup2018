
Dispenser::Dispenser(int pin){
}
Dispenser::Dispenser(int startAmount){
	amountAvailable = startAmount;
}
void Dispenser::setAmountAvailable(int iAmount){
	amountAvailable = iAmount;
}
int Dispenser::getAmountAvailable(){
	return amountAvailable;
}
int Dispenser::updateAmountAvailable(){
	amountAvailable--;
	return amountAvailable;
}

void Dispenser::dispenseDirection(DispenserDirection direction, int amount){
	if(direction == left && kitsAvailable()){
		for(int i =0; i< amount; i++){
			servoDispenserwrite(45);
			delay(1000);
			servoDispenser.write(90);
			servoDispenser.updateAmountAvailable(1);
		}
	}else if(direction == right && kitsAvailable()){
		for(int i =0; i<amount; i++){
			servoDispenser.write(135);
			delay(1000);
			servoDispenser.write(90);
			servoDispenser.updateAmountAvailable(1)
		}
	}else{
		servoDispenser.write(90);
	}
	
}

bool Dispenser::kitsAvailable(){
	if(getAmountAvailable() > 0)
		return true;
	else 
		return false;
}