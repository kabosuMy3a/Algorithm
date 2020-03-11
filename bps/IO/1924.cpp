#include <iostream>

class dayCal{

	public: 
		int month = 0, day=0;
		int num =0;

		dayCal(){}
		int cal(int month, int day){
			switch(month){
				case 3 :
				case 5 :
				case 7 :
				case 8 :
				case 10 :
				case 12 :	
					num+= 31; 
					cal(month-1,day);
					break;
				case 2 : 
					num += 28; 
					cal(month-1,day);
					break;
				case 4 :
				case 6 :
				case 9 :
				case 11:
				       	num += 30;
			      	       	cal(month-1,day);
				       	break;	       
			}	
			return num;
		}
};

int main(){

	dayCal * dc = new dayCal();
	std::cin >>dc-> month >> dc->day;

	dc->cal(dc->month,dc->day);
	dc->num += dc->day;
	
	switch((dc->num)%7){
		case 1: std::cout << "MON" << std::endl; break;
		case 2: std::cout << "TUE" << std::endl; break;
		case 3: std::cout << "WED" << std::endl; break;
		case 4: std::cout << "THU" << std::endl; break;
		case 5: std::cout << "FRI" << std::endl; break;
		case 6: std::cout << "SAT" << std::endl; break;
		case 0: std::cout << "SUN" << std::endl; break;
	}

	std::cout << dc->num << std::endl;
	

	return 0;
}
