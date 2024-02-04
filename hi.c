#include <stdio.h>
#include <stdlib.h>	//both libraries used for generating random numbers
#include <time.h>

typedef enum {
	noone,	//noone=0
	cap,	//CAP=1
	car 	//CAR=2
}player_type;

typedef enum {
	start, property, tax, punish, fortune
}block_type;

struct player{	
	player_type type;			//Holds type of the player. The player_type must be defined as enumerated type which consist of noone, cap and car values.
	int current_block_id;		//Holds player location as block id.
	int owned_block_ids[12]; 	//Holds property block ids that are owned by the user.								
    int account;  				//Holds amount of the current money of the player.
    int turn_to_wait;  			//Holds number of turns to wait if the player got a punishment.
    char * name; 				//Holds the name of the player.	
};
struct block{	
 	int block_id; 				//Holds id of the block.	
 	char * name;  		 		//Holds text of the block that is shown on the top of the block.
	int price;					//If the block is property, holds the price of property. If the block is a tax....
	int rent;					//Holds the default rent of a property (no house).
	int rent_1;					//Holds the rent of a property with one house.
	int rent_2;					//Holds the rent of a property with two houses.
	int rent_3;					//Holds the rent of a property with three houses.
	int house_price;			//Holds price of building a house on the block.
	int house_count; 			//Holds the number of the houses on the block that are already built.
	struct player player_owner; //Holds the owner of the block. 
	block_type type; 			//Holds type of the block. 
	struct block *next;
};

struct fortune_card{	//пока не знаю для чего буду использовать
	char * name;
	int card_no;
	block_type c_type;
	struct fortune_card *link;
};
	//FUNCTIONS PROTOTYPES
void init_the_board(struct block *board);
void init_fortune_struct(struct fortune_card *fortune);
	//FOR game
void game_MAIN(struct block *board, struct player player_one,struct player player_two);
int start_turn(struct block *board,struct player* player_one, struct player* player_two, int turn);
int check_end(struct player current_player);


void roll_d_AI(struct block *board, struct player* player_one, struct player* player_two );
void gameplay (struct block *board, struct player* player_one, struct player* player_two );
void show_my_properties(struct block *board, struct player current_player);
void show_properties(struct block *board);
void print_property(struct block *board);
void after_dice_menu_AI(struct block *board, struct player* current_player, struct player* player_two );
	//FOR printing
void show_board(struct block *board,struct player player_one, struct player player_two);
void print_menu();

	//USER
void buy_property_user(struct block* board, struct player* current_player);
void buy_house_user(struct block* board, struct player* current_player);
void sell_property_user(struct block *board, struct player* current_player);
void after_dice_menu_user(struct block *board, struct player* current_player,struct player* player_one);
void pay_rent_user(struct block *board,struct player* current_player,struct player*  owner);
int need_money(struct block *board, struct player* current_player,int );
void fortune_user(struct block *board,struct player* current_player, struct player* player_one);

	//AI
int count_property(struct player current_player);
void buy_property_AI( struct player* current_player, struct block *board );
void buy_house_AI(struct block* board, struct player* current_player);
void pay_rent_AI(struct block *board, struct player* current_player, struct player* player_two);
void pay_tax_AI(struct block *board, struct player* current_player);
void fortune_AI(struct block *board,struct player* current_player,int dice, struct player* player_two);
void house_dialog(struct block* board, struct player* current_player);
void sell_property_AI(struct block *board, struct player* current_player);
int check_property_AI( struct player current_player, struct block *board);
int sum_avg(struct block *board);


int main(){
	struct block * board;
	struct fortune_card *fortune;
	struct player player_one;
	struct player player_two;
	int i;

	board=(struct block*)malloc(sizeof(struct block));
	fortune=(struct fortune_card*)malloc(sizeof(struct fortune_card));

	init_the_board(board);		//Making initialization of the board.
	init_fortune_struct(fortune);

	srand(time(NULL));			//For taking random numbers without sequenced order.

	for(i=0; i<12; i++){						//I declared -1 to every element of an array in the beginning
		player_one.owned_block_ids[i]=-1;	//For controlling it easier, later.
		player_two.owned_block_ids[i]=-1;
	}

	game_MAIN(board,player_one,player_two);
	
	while(board!=NULL){		//deallocating the memory
		free(board);
		board=board->next;
	}
	while(fortune!=NULL){
		free(fortune);
		fortune=fortune->link;
	}

	return 0;
}

int str_length(char *length){	//Small function for finding length of the string. I used it, for printing board to the console.
	int i, count=0;

	for(i=0; i<15 && length[i]!='\0'; i++){
		count++;
	}
	return count;
}
int num_length(int number){
	int i, count=0;

	while(number > 0){
     number = number / 10;
     count++; 
  }
	return count;
}

void init_the_board(struct block *board){

	board->block_id=0;	
	board->name="Start";
	board->price=0;
	board->rent=0;
	board->rent_1=0;
	board->rent_2=0;
	board->rent_3=0;
	board->house_price=0;
	board->house_count=0;
	board->type=start;
	board->player_owner.type=noone;

	board->next=(struct block*)malloc(sizeof(struct block));
	board=board->next;

	board->block_id=1;	
	board->name="Esenyurt";
	board->price=16000;
	board->rent=800;
	board->rent_1=4000;
	board->rent_2=9000;
	board->rent_3=25000;
	board->house_price=10000;
	board->house_count=0;
	board->type=property;
	board->player_owner.type=noone;
	board->next=(struct block*)malloc(sizeof(struct block));
	board=board->next;


	board->block_id=2;	
	board->name="Car Park";
	board->price=0;
	board->rent=1500;
	board->rent_1=0;
	board->rent_2=0;
	board->rent_3=0;
	board->house_price=0;
	board->house_count=0;
	board->type=tax;
	board->player_owner.type=noone;
	board->next=(struct block*)malloc(sizeof(struct block));
	board=board->next;


	board->block_id=3;	
	board->name="Fortune Card";
	board->price=0;
	board->rent=0;
	board->rent_1=0;
	board->rent_2=0;
	board->rent_3=0;
	board->house_price=0;
	board->house_count=0;
	board->type=fortune;
	board->player_owner.type=noone;
	board->next=(struct block*)malloc(sizeof(struct block));
	board=board->next;

	board->block_id=4;	
	board->name="Tuzla";
	board->price=16500;
	board->rent=850;
	board->rent_1=4250;
	board->rent_2=9500;
	board->rent_3=26000;
	board->house_price=12000;
	board->house_count=0;
	board->type=property;
	board->player_owner.type=noone;
	board->next=(struct block*)malloc(sizeof(struct block));
	board=board->next;

	board->block_id=5;	
	board->name="Arnavutkoy";
	board->price=17000;
	board->rent=875;
	board->rent_1=4500;
	board->rent_2=10000;
	board->rent_3=28000;
	board->house_price=12000;
	board->house_count=0;
	board->type=property;
	board->player_owner.type=noone;
	board->next=(struct block*)malloc(sizeof(struct block));
	board=board->next;

	board->block_id=6;	
	board->name="Wait 2 Turn";
	board->price=0;
	board->rent=2;
	board->rent_1=0;
	board->rent_2=0;
	board->rent_3=0;
	board->house_price=0;
	board->house_count=0;
	board->type=punish;
	board->player_owner.type=noone;
	board->next=(struct block*)malloc(sizeof(struct block));
	board=board->next;

	board->block_id=7;	
	board->name="Catalca";
	board->price=20000;
	board->rent=1000;
	board->rent_1=5000;
	board->rent_2=12000;
	board->rent_3=30000;
	board->house_price=13000;
	board->house_count=0;
	board->type=property;
	board->player_owner.type=noone;
	board->next=(struct block*)malloc(sizeof(struct block));
	board=board->next;

	board->block_id=8;	
	board->name="Beykoz";
	board->price=23000;
	board->rent=1100;
	board->rent_1=5500;
	board->rent_2=12500;
	board->rent_3=33000;
	board->house_price=13000;
	board->house_count=0;
	board->type=property;
	board->player_owner.type=noone;
	board->next=(struct block*)malloc(sizeof(struct block));
	board=board->next;

	board->block_id=9;	
	board->name="Fortune Card";
	board->price=0;
	board->rent=0;
	board->rent_1=0;
	board->rent_2=0;
	board->rent_3=0;
	board->house_price=0;
	board->house_count=0;
	board->type=fortune;
	board->player_owner.type=noone;
	board->next=(struct block*)malloc(sizeof(struct block));
	board=board->next;

	board->block_id=10;	
	board->name="Car Fix";
	board->price=0;
	board->rent=1750;
	board->rent_1=0;
	board->rent_2=0;
	board->rent_3=0;
	board->house_price=0;
	board->house_count=0;
	board->type=tax;
	board->player_owner.type=noone;
	board->next=(struct block*)malloc(sizeof(struct block));
	board=board->next;

	board->block_id=11;	
	board->name="Maltepe";
	board->price=30000;	 
	board->rent=1350;
	board->rent_1=7000;
	board->rent_2=15000;
	board->rent_3=40000;
	board->house_price=15000;
	board->house_count=0;
	board->type=property;
	board->player_owner.type=noone;
	board->next=(struct block*)malloc(sizeof(struct block));
	board=board->next;

	board->block_id=12;	
	board->name="Bills";	 	
	board->price=0;	 
	board->rent=2000;
	board->rent_1=0;
	board->rent_2=0;
	board->rent_3=0;
	board->house_price=0;
	board->house_count=0;
	board->type=tax;
	board->player_owner.type=noone;
	board->next=(struct block*)malloc(sizeof(struct block));
	board=board->next;

	board->block_id=13;	
	board->name="Sisli";		 
	board->price=33000;	 
	board->rent=1500;
	board->rent_1=8000;
	board->rent_2=16000;
	board->rent_3=42000;
	board->house_price=16000;
	board->house_count=0;
	board->type=property;
	board->player_owner.type=noone;
	board->next=(struct block*)malloc(sizeof(struct block));
	board=board->next;

	board->block_id=14;	
	board->name="Oil";
	board->price=0;	 		
	board->rent=2250;
	board->rent_1=0;
	board->rent_2=0;
	board->rent_3=0;
	board->house_price=0;
	board->house_count=0;
	board->type=tax;
	board->player_owner.type=noone;
	board->next=(struct block*)malloc(sizeof(struct block));
	board=board->next;

	board->block_id=15;	
	board->name="Fortune Card";
	board->price=0;
	board->rent=0;
	board->rent_1=0;
	board->rent_2=0;
	board->rent_3=0;
	board->house_price=0;
	board->house_count=0;
	board->type=fortune;
	board->player_owner.type=noone;
	board->next=(struct block*)malloc(sizeof(struct block));
	board=board->next;

	board->block_id=16;	
	board->name="Atasehir";		 	 		 		 	
	board->price=35000;	 
	board->rent=1600;
	board->rent_1=8500;
	board->rent_2=17000;
	board->rent_3= 45000;
	board->house_price=17000;
	board->house_count=0;
	board->type=property;
	board->player_owner.type=noone;
	board->next=(struct block*)malloc(sizeof(struct block));
	board=board->next;

	board->block_id=17;	
	board->name="Sariyer"; 
	board->price=40000;	 
	board->rent=1750;
	board->rent_1=9500;
	board->rent_2=19000;
	board->rent_3=48000;
	board->house_price=19000;
	board->house_count=0;
	board->type=property;
	board->player_owner.type=noone;
	board->next=(struct block*)malloc(sizeof(struct block));
	board=board->next;

	board->block_id=18;	
	board->name="Wait 1 Turn";
	board->price=0;	 		 
	board->rent=1;
	board->rent_1=0;
	board->rent_2=0;
	board->rent_3=0;
	board->house_price=0;
	board->house_count=0;
	board->type=punish;
	board->player_owner.type=noone;
	board->next=(struct block*)malloc(sizeof(struct block));
	board=board->next;

	board->block_id=19;	
	board->name="Kadikoy";		
	board->price=43000;	 
	board->rent=1900;
	board->rent_1=11000;
	board->rent_2=21500;
	board->rent_3=55000;
	board->house_price=23000;
	board->house_count=0;
	board->type=property;
	board->player_owner.type=noone;
	board->next=(struct block*)malloc(sizeof(struct block));
	board=board->next;

	board->block_id=20;			
	board->name="Besiktas";
	board->price=60000;	 
	board->rent=2500;
	board->rent_1=15000;
	board->rent_2=28000;
	board->rent_3=60000;
	board->house_price=30000;
	board->house_count=0;
	board->type=property;
	board->player_owner.type=noone;
	board->next=(struct block*)malloc(sizeof(struct block));
	board=board->next;

	board->block_id=21;	
	board->name="Fortune Card";
	board->price=0;
	board->rent=0;
	board->rent_1=0;
	board->rent_2=0;
	board->rent_3=0;
	board->house_price=0;
	board->house_count=0;
	board->type=fortune;
	board->player_owner.type=noone;
	board->next=(struct block*)malloc(sizeof(struct block));
	board=board->next;

	board->block_id=22;	
	board->name="Vocation";		 
	board->price=0;	 
	board->rent=5000;
	board->rent_1=0;
	board->rent_2=0;
	board->rent_3=0;
	board->house_price=0;
	board->house_count=0;
	board->type=tax;
	board->player_owner.type=noone;
	board->next=(struct block*)malloc(sizeof(struct block));
	board=board->next;	

	board->block_id=23;	
	board->name="Bebek";	 	 
	board->price=70000;	 
	board->rent=3500;
	board->rent_1=20000;
	board->rent_2=35500;
	board->rent_3=65000;
	board->house_price=35000;
	board->house_count=0;
	board->type=property;
	board->player_owner.type=noone;

}

void init_fortune_struct(struct fortune_card *fortune){
	
	fortune->name="Free House";
	fortune->card_no=1;
	
	fortune->link=(struct fortune_card*)malloc(sizeof(struct fortune_card));
	fortune=fortune->link;	

	fortune->card_no=2;
	fortune->name="Time Travel";
	fortune->link=(struct fortune_card*)malloc(sizeof(struct fortune_card));
	fortune=fortune->link;

	fortune->card_no=3;
	fortune->name="Garnishment";
	fortune->link=(struct fortune_card*)malloc(sizeof(struct fortune_card));
	fortune=fortune->link;

	fortune->card_no=4;
	fortune->name="Generosity";
	fortune->link=(struct fortune_card*)malloc(sizeof(struct fortune_card));
	fortune=fortune->link;

	fortune->card_no=5;
	fortune->name="Treasure Hunter";

}	


void game_MAIN(struct block *board,struct player player_one, struct player player_two){
	struct player current_player;	
	struct block current_block;
	int is_end=0;
	int turn=1;
	struct block *mainBoard=board;
	player_one.name="Cap";			//AI
	player_one.type=cap;
	player_one.current_block_id=0;	
	player_one.account=100000;	
    player_one.turn_to_wait=0;
    current_player=player_one;		//At the beginning of the game turn and current player - Computer.

    player_two.name="Car";			//User
	player_two.type=car;
	player_two.current_block_id=0;
    player_two.turn_to_wait=0;
	player_two.account=100000;

	printf("\t\t\t\t\t\t\t\t\e[5;31;47mWelcome to Monopoly 2!\e[0m\n");

	printf("Player %s will roll the dice first..\n", player_one.name);
	printf("\e[5;31;47mPLAYER -> %s\e[0m\n", player_one.name);
	printf("%s:%d$ , %s:%d$\n",player_one.name, player_one.account, player_two.name, player_two.account);
 	show_board(board,player_one,player_two);
    while(is_end!=1)
    {
        turn=start_turn(board,&player_one,&player_two, turn);
        show_board(board,player_one,player_two);
        is_end=check_end(player_one);
        
        if (is_end==0)
        {
        	
            printf("%s :%d$ , %s:%d$\n",player_one.name,player_one.account,player_two.name,player_two.account);
            turn=start_turn(board,&player_one,&player_two, turn);
            show_board(board,player_one,player_two);

            is_end=check_end(player_two);
            if (is_end==1)
                printf("%s don't have enough money to payment, unfortunately %s lost the game!\n",player_two.name, player_two.name);
        } else
            printf("%s don't have enough money to payment, unfortunately %s lost the game!\n",player_one.name, player_one.name);
         
    }
}

void show_board(struct block *board,struct player player_one, struct player player_two){

	int i,j,k,n,z, m=23, p=23, o=23;
	int name=16, price=16, own=16;
	int flag=0;
	int end=5;
	struct block *front;	//start shows beginning of the linked list

	front=board;		
	for(i=0; i<176; i++){	//'-' for the first line
		printf("-");
	}
	printf("\n");

		for(i=0; i<7; i++){	
			printf("|");			//For the first line of names
			if(str_length(board->name)==12) printf("%16s        ",board->name);
			else if(str_length(board->name)==11) printf("%17s       ",board->name);
			else if(str_length(board->name)==10) printf("%17s       ",board->name);
			else if(str_length(board->name)==8) printf("%16s        ",board->name);
			else if(str_length(board->name)==7) printf("%15s         ",board->name);
			else if(str_length(board->name)==6) printf("%15s         ",board->name);
			else if(str_length(board->name)==5) printf("%14s          ",board->name);
			else if(str_length(board->name)==3) printf("%13s           ",board->name);
			board=board->next;
		}
		printf("|");
		printf("\n");

		board=front;
		for(i=0; i<7; i++){	//For the second line of price

			printf("|");

			if(board->type==tax){		//If type of location is tax we need to print rent price of the place
				if(num_length(board->rent)==4) printf("%14d$         ",board->rent);
				
			}
			else if(board->type==property){	//If type is property we have to print price of the block
				if(num_length(board->price)==5) printf("%14d$         ",board->price);
				else if(num_length(board->price)==4) printf("%14d$         ",board->price);
			}
			else {	
				printf("                        ");
			}
			board=board->next;		
		}
		printf("|");
		printf("\n");

		board=front;
		for(i=0; i<7; i++){	//Third line for Car and Cap.
			printf("|");			
			if(player_one.current_block_id!=board->block_id && player_two.current_block_id!=board->block_id) {
				if(board->type==fortune)	printf("                        ");	
				else 	printf("                        ");	
			}
			else if(player_one.current_block_id==board->block_id && player_two.current_block_id==board->block_id){
				printf("%9s     ", player_one.name);
				printf("%s      ", player_two.name);
			}
			else if(player_one.current_block_id==board->block_id){	
				if(board->type==fortune) printf("%13s          ", player_one.name);
				else printf("%13s           ", player_one.name);
			}
			else if(player_two.current_block_id==board->block_id){
				if(board->type==fortune) printf("%13s          ", player_two.name);
				else printf("%13s           ", player_two.name);
			}
			board=board->next;
		}
		printf("|");
		printf("\n");
				
		for(i=0; i<176; i++){	//For '-' after first 6 columns
			printf("-");
		}
		printf("\n");		

		//Next step, VERTICAL COLUMNS AND SPACE BETWEEN THEM.

		/*According to my algorithm, for printing vertical columns I need to go in this way like: 
		23->22->21->20..(from the left)
		7->8->9->10..	(from the right)
		So I throught that: I can make an equation for this such:
		In the beginning m=23.	name=16. (Started from 16, because I needed to substract 16 for obtaining 7)
		7=23-16;	(16 is variable (name), which decreasing everytime like 16,15,14,13.. making: name-- )
		22=7+15;
		8=22-14;
		21=8+13;
		9=21-12;
		...
		*/

		while(end!=0){	//The main loop, continuous 5 times(for 5 rows), that's why end is 5 in the beginning
			for(j=0;j<2; j++){	//LOOP FOR PRINTING NAMES OF LOCATIONS.	
				board=front;
				for(k=0; k<m; k++){
					board=board->next;
				}	
				//printf("%d", board->block_id);
				printf("|");

					if(str_length(board->name)==12) printf("%18s      ",board->name);
					else if(str_length(board->name)==11) printf("%17s       ",board->name);
					else if(str_length(board->name)==10) printf("%17s       ",board->name);
					else if(str_length(board->name)==8) printf("%16s        ",board->name);
					else if(str_length(board->name)==7) printf("%15s         ",board->name);
					else if(str_length(board->name)==6) printf("%15s         ",board->name);
					else if(str_length(board->name)==5) printf("%14s          ",board->name);
					else if(str_length(board->name)==3) printf("%13s           ",board->name);
				printf("|");
				if(flag==1){	//I used flags, for controlling, when I have to put spaces and when newline
					printf("\n");
				}
				if(flag==0){	//for printing spaces between.
					for(i=0; i<124; i++){	
						printf(" ");
					}
					flag=1;
					m=m-name;	
				}			
			}
			name--;		
			m=m+name;	
			name--;
			flag=0;
		
		for(j=0; j<2; j++){	//LOOP FOR PRINTING PRICE.
			board=front;
			for(n=0; n<p; n++){
				board=board->next;
			}
			printf("|");
				
			if(board->type==tax){		//If type of location is tax we need to print rent price of the place
				if(num_length(board->rent)==4) printf("%14d$         ",board->rent);		
			}			
		
			else if(board->type==property){	//If type is property we have to print price of the block
				if(num_length(board->price)==5) printf("%14d$         ",board->price);
				else if(num_length(board->price)==4) printf("%14d$         ",board->price);
			}

			else{ 		//Else, we don't need to print price.
				 printf("                        ");
				}
			printf("|");

			if(flag==1){
				printf("\n");
			}
			if(flag==0){	//for printing spaces between.
				for(i=0; i<124; i++){	
					printf(" ");			
				}
				flag=1;
				p=p-price;
			}

		}
			price--;
			p=p+price;
			price--;
			flag=0;
			


		for(j=0; j<2;j++){	//LOOP FOR CAR AND CAP.
			board=front;
			for(z=0; z<o; z++){
				board=board->next;
			}
				printf("|");	
				 if(player_one.current_block_id!=board->block_id && player_two.current_block_id!=board->block_id) {
				 	printf("                        ");	
				 }
				else if(player_one.current_block_id==board->block_id && player_two.current_block_id==board->block_id){
					printf("%9s     ", player_one.name);
					printf("%s       ", player_two.name);
				}
		
				else if(player_one.current_block_id==board->block_id){		
					printf("%13s           ", player_one.name);
				}
				else if(player_two.current_block_id==board->block_id){
					printf("%13s           ", player_two.name);
				}
				printf("|");
				if(flag==1){
					printf("\n");
				}
				if(flag==0){	//for printing spaces between.
					for(i=0; i<124; i++){	
						printf(" ");
					}
					flag=1;
					o=o-own;	//for printing name of the next one. means(For example 19-13=6)
				}			
			}
			own--;
			o=o+own;
			own--;	
			flag=0;

			if(end==1){		//If it is the last iteration of the loop then I should print '-' only (without spaces)
				for(i=0; i<176; i++){	
					printf("-");
				}
				printf("\n");
			}
			else{
				for(i=0; i<26; i++){	//For '-' after first line
					printf("-");
				}
				for(i=0; i<124; i++){	//For space between
					printf(" ");
				}
				for(i=0; i<26; i++){	//For '-' after second line
					printf("-");
				}
				printf("\n");
			}
			end--;
		}
	
		// LAST 'FOR' LOOP, FOR LAST ROW OF BLOCKS (12-18)
		
		for(i=18; i>=12; i--){	
			board=front;
			for (k = 0; k < i; k++){
				board=board->next;
			}
			printf("|");	
			if(str_length(board->name)==12) printf("%16s        ",board->name);		
			else if(str_length(board->name)==11) printf("%17s       ",board->name);
			else if(str_length(board->name)==10) printf("%17s       ",board->name);
			else if(str_length(board->name)==8) printf("%16s        ",board->name);
			else if(str_length(board->name)==7) printf("%15s         ",board->name);
			else if(str_length(board->name)==6) printf("%15s         ",board->name);
			else if(str_length(board->name)==5) printf("%14s          ",board->name);
			else if(str_length(board->name)==3) printf("%13s           ",board->name);

		}
		printf("|");
		printf("\n");

		
		for(i=18; i>=12; i--){
			board=front;
			for (k = 0; k < i; k++){
				board=board->next;
			}	
			printf("|");

			if(board->type==tax){		//If type of location is tax we need to print rent price of the place
				if(num_length(board->rent)==4) printf("%14d$         ",board->rent);		
			}			
			else if(board->type==property){	//If type is property we have to print price of the block
				if(num_length(board->price)==5) printf("%14d$         ",board->price);
				else if(num_length(board->price)==4) printf("%14d$         ",board->price);
			}
			else{ 		//Else, we don't need to print price.
				 printf("                        ");
				}
		}
		printf("|");
		printf("\n");				
					
		for(i=18; i>=12; i--){	//Third line for Car and Cap.
			board=front;
			for (k = 0; k < i; k++){
				board=board->next;
			}
			printf("|");	

			if(player_one.current_block_id!=board->block_id && player_two.current_block_id!=board->block_id) {
				if(board->type==fortune)	printf("                        ");	
				else 	printf("                        ");	
			}
			else if(player_one.current_block_id==board->block_id && player_two.current_block_id==board->block_id){
				printf("%9s     ", player_one.name);
				printf("%s      ", player_two.name);
			}
			else if(player_one.current_block_id==board->block_id){	
				if(board->type==fortune) printf("%13s          ", player_one.name);
				else printf("%13s           ", player_one.name);
			}
			else if(player_two.current_block_id==board->block_id){
				if(board->type==fortune) printf("%13s          ", player_two.name);
				else printf("%13s           ", player_two.name);
			}
		}
		printf("|");
		printf("\n");
				
		for(i=0; i<176; i++){	//For '-' after first 6 columns
			printf("-");
		}
		printf("\n");
}

int start_turn(struct block *board, struct player* player_one, struct player* player_two, int turn){
	int i, flag=1;
	struct block *front;
	//int real_dice, finish=0;
	printf("TWO%d\n", player_two->turn_to_wait);
	printf("%d\n", player_one->turn_to_wait);
	front=board;
	while(flag==1){
		if(turn==1){			//IF AI turn
		
			if(player_one->turn_to_wait==0 && turn==1){	//If it is first player's turn, roll the dice and change the turn to 2
				//printf("\e[5;31;47mComputer rolled the dice N:  %d\e[0m\n",real_dice);		
				roll_d_AI(board, player_one, player_two);
				flag=0;
				turn=2;
			}	
			else if(player_one->turn_to_wait!=0 && turn==1){
				player_one->turn_to_wait-=1;
				flag=0;
				turn=2;
			}
		}
		else if(turn==2){		//IF User's turn
			gameplay(board, player_one, player_two);
			flag=0;	
			turn=1;
		}
		printf("Turn>>>%d\n",turn );

	}
	return turn;
}

int check_end(struct player current_player){
    int owned_property_count=count_property(current_player);
    if (current_player.account<=0 && owned_property_count==0)return 1;
    else return 0;
}

void roll_d_AI(struct block *board, struct player *player_one, struct player *player_two){
	struct block * front;

	int i, dice, real_dice;
	front=board;
	dice = 1 + (rand() % 6);
		real_dice=dice;
		
	if(19-player_one->current_block_id<dice){	//Next tour started
		dice=dice-(19-player_one->current_block_id)-1;
		player_one->current_block_id=0;
		player_one->account=player_one->account+10000;	//Everytime after coming to start, money will increase to 10000
	}
		player_one->current_block_id=player_one->current_block_id+dice;	

		printf("\e[5;31;47mComputer rolled the dice: N  %d\e[0m\n",real_dice);
		printf("\e[5;31;47mPLAYER -> %s\e[0m\n", player_one->name);
		//show_board(board, player_one, player_two);

	    after_dice_menu_AI(board,player_one, player_two);

		printf("\e[5;31;47mPLAYER -> %s\e[0m\n", player_two->name);

}

void gameplay (struct block *board, struct player* player_one, struct player* player_two){
	struct block *front;
	int dice, real_dice, selection;
	struct player *current_player;
	struct block current_block;
	front=board;
	int i, status=0;
	while(status==0){
		print_menu();
		current_player=player_two;

		
		scanf("%d", &selection);
			
			
		switch(selection){
			case 1:	
				srand(time(NULL));	//For taking random numbers without sequenced order.
				if (current_player->turn_to_wait==0) {
					dice = 1 + (rand() % 6);
					real_dice=dice;
					
					if(19-player_two->current_block_id<dice){
					dice=dice-(19-player_two->current_block_id)-1;
					player_two->current_block_id=0;
					player_two->account=player_two->account+10000;
					}
			
					player_two->current_block_id=player_two->current_block_id+dice;
					
					printf("\e[5;31;47mDice of %s--> %d\e[0m\n",player_two->name, real_dice);
					printf("\e[5;31;47mPLAYER -> %s\e[0m\n", player_two->name);
					show_board(board, *player_one, *player_two);

					current_player=player_two;
					after_dice_menu_user(front, current_player, player_one);
				}
				else
	                {
	                    printf("You need to wait %d turn to continue.\n",current_player->turn_to_wait);
	                    current_player->turn_to_wait-=1;
	                }
	                
				printf("\e[5;31;47mPLAYER -> %s\e[0m\n", player_one->name);
				
				status=1;
			break;
			case 2:
			
				current_player=player_two;
				printf("Your account is:%d$\n", current_player->account); 
			
			break;
			case 3:
				show_my_properties(front, *player_two);
			
			break;
			case 4:
				show_properties(front);		
			break;
			case 5:
				 current_player=player_two;	
					
				buy_property_user(front, player_two);

			break;
			case 6:

				buy_house_user(front, player_two);	
			
			break;
			case 7:
				sell_property_user(front, player_two);

			break;
			default:
			printf("%s","\e[5;31;47mThis is an invalid choice. Try again!\e[0m\n");
		}
		board=front;
	}
}

void print_menu(){
	printf("%s","1 - Roll the dice\n");
	printf("%s","2 - Show my account\n");		
	printf("%s","3 - Show my properties\n");
	printf("%s","4 - Show property deeds\n");
	printf("%s","5 - Buy property\n");
	printf("%s","6 - Buy house\n");		
	printf("%s","7 - Sell property\n");
	printf("%s","8 - Exit\n");
	printf("%s","\e[5;31;47mPlease select an option to continue\e[0m\n");

}

void show_my_properties(struct block *board, struct player current_player){
	int i=0, k=0,j=0, m;
	int owned;
	struct block *front;	//start shows beginning of the linked list
	front=board;
	if(current_player.owned_block_ids[0]==-1){
		printf("For now, you don't have any owned property..\n");
	}
	else{
			printf("Properties owned by you: \n");
		while(current_player.owned_block_ids[i]!=-1 ){	//For  printing properties owned by user

			owned=current_player.owned_block_ids[i];
			board=front;
			for(m=0; m<owned; m++){
				front=front->next;
			}
			printf("%d. Block-%d  Name -'%s'  Price- %d$ \n", i+1, current_player.owned_block_ids[i], front->name, front->price);
			while(front->house_count > k){	//For printing houses owned by user.
				printf("%d. House price %d\n",j+2+k, front->house_price);
				k++;
			}
			k=0;
			i++;
			j++;
		}

	}
	printf("\n");
}

	//PART 3  (For 4 case "Show property deeds")
void show_properties(struct block *board){
	int i;
	int choice;	//For keeping value chosen by the user.
	struct block *front;	//start shows beginning of the linked list

	printf("%s","\e[5;31;47mPlease select a property to see details: \e[0m\n");
	printf("1 -  Esenyurt\n4 -  Tuzla\n5 -  Arnavutkoy\n7 -  Catalca\n8 -  Beykoz\n11 -  Maltepe\n13 - Sisli\n16 - Atasehir\n17 - Sariyer\n19 - Kadikoy\n20 - Besiktas\n23 - Bebek\n0 -  Exit\n");

	front=board;
	scanf("%d", &choice);
	while(choice!=0){	//If user types 0 ->exit
		front=board;	//For starting from the beginning next time
		for(i=0; i<choice; i++){
			front=front->next;
		}
		if(front->type==property) print_property(front);	
		else {printf("Please select a correct properties number, according to the list shown below.\n\n");}
		printf("%s","\e[5;31;47mPlease select a property to see details: \e[0m\n");
		printf("1 -  Esenyurt\n4 -  Tuzla\n5 -  Arnavutkoy\n7 -  Catalca\n8 -  Beykoz\n11 -  Maltepe\n13 - Sisli\n16 - Atasehir\n17 - Sariyer\n19 - Kadikoy\n20 - Besiktas\n23 - Bebek\n0 -  Exit\n");
		scanf("%d", &choice);
	}
	
}
	//Helper function for printing table of properties.
void print_property(struct block *board){
	int i, j;

	for(i=0; i<4; i++){		//Continues 4 times for printing '-'
		for(j=0; j<33; j++){
			printf("-");
		}
		printf("\n");

		if(i==0){
		    if(str_length(board->name)==10) printf("|%20s            |\n",board->name);
			else if(str_length(board->name)==8) printf("|%20s            |\n",board->name);
			else if(str_length(board->name)==7) printf("|%20s            |\n",board->name);
			else if(str_length(board->name)==6) printf("|%20s             |\n",board->name);
			else if(str_length(board->name)==5) printf("|%19s             |\n",board->name);
		}

		if(i==1){
			if(num_length(board->rent)==4) printf("|     Rent        %d$          |\n",board->rent);
			else if(num_length(board->rent)==3) printf("|     Rent        %d$           |\n",board->rent);
			if(num_length(board->rent_1)==5) printf("|     Rent 1 H    %d$         |\n",board->rent_1);
			else if(num_length(board->rent_1)==4) printf("|     Rent 1 H    %d$          |\n",board->rent_1);
			if(num_length(board->rent_2)==5) printf("|     Rent 2 H    %d$         |\n",board->rent_2);
			else if(num_length(board->rent_2)==4) printf("|     Rent 2 H    %d$          |\n",board->rent_2);
			printf("|     Rent 3 H    %d$         |\n",board->rent_3);	
		}

		if(i==2){
			printf("|     House Price   %d$       |\n",board->house_price);
		}	
	}
}

void after_dice_menu_AI(struct block *board, struct player* current_player, struct player* player_two){
    int dice,i;
struct block *front=board;

	for(i=0; i<current_player->current_block_id; i++){
		front=front->next;
	}

    switch (front->type)
    {
        case property:
            printf("%s has arrived %s \n",current_player->name,front->name);
           
			if(front->player_owner.type==noone && front->type==property){	
	
				buy_property_AI( current_player, board);
			}
			else if(front->player_owner.type!=noone && front->type==property){
				if(front->player_owner.type==cap){

					buy_house_AI(board ,current_player);
				}
				else if(front->player_owner.type==car){
	
					pay_rent_AI(board, current_player, player_two);
				}
			}
            break;
        case tax:
            printf("%s has arrived tax block(%s)\n",current_player->name,front->name);

            pay_tax_AI(board, current_player);
            break;
        case punish:
            printf("%s has punished for %d turn. \n",current_player->name, front->rent);
            current_player->turn_to_wait+=front->rent;
            break;
        case fortune:
        	dice = 1 + (rand() % 5);

        	fortune_AI(board, current_player, dice, player_two);
        	break;
        case start:
        	break;
    }
}

void buy_property_user(struct block* board, struct player* current_player){
	int i, j,m;
	int flag=0;
	struct block* front;
	front=board;

	for(m=0;m<current_player->current_block_id ;m++){
		front=front->next;
	}
	printf("Block id -> %d Name -> %s\n", front->block_id, front->name);
	printf("Current player -> %s\n", current_player->name);
	printf("Player's account -> %d$\n", current_player->account);

	if(front->type!=property){ 	//Checking is it a property or not
		printf("You are not in a property typed place! Please come later :)\n");
	}
	else if(current_player->account < front->price){	//Checking do the user have enough money or not
		printf("Unfortunately, you don't have enough money to buy this property..\n");
	}
	else{
		for(i=0; i<12 && flag!=1; i++){		//Firstly, I have to check if this property is free or not. 
			if(current_player->owned_block_ids[i]==front->block_id){	//So this loop checks all elements and if the property is free ->buy it.
				printf("This property place is already owned ! Please check it later :)\n");
			flag=1;
			}

			if(i==11 && flag==0){	//If this contidion is satisfied, that means we can buy a property.
				for(j=0; j<12 && flag==0; j++){
					if(current_player->owned_block_ids[j]==-1){
						printf("Congratulations! You bought this property !\n");
						current_player->owned_block_ids[j]=front->block_id;
						current_player->account=current_player->account - front->price;	//Substacting money of player.
						front->player_owner=*current_player;	//Copying all information of the current block to PLayer Owner of the according block
						printf("You new account -> %d$\n", current_player->account);
					flag=1;
					}
					else if(j==11 && flag==0){	//I don't think this condition will be satisfied, but however I guess it is better to have it
						printf("All properties are already yours! :)\n");
					}
				}		
			}
		}
	}
}


	//Helper function for bying a house by user.
void buy_house_user(struct block* board, struct player* current_player){
	int answer,m;
	struct block* front;
	for(m=0;m<current_player->current_block_id;m++){
		front=front->next;
	}

	printf("Block id -> %d Name -> %s\n", front->block_id, front->name);
	printf("Current player -> %s\n", current_player->name);
	printf("Player's account -> %d$\n", current_player->account);

	if(front->type!=property){ 	//Checking is it a property or not
		printf("You are not in a property typed place! Please come later :)\n");
	}
	else if(front->player_owner.type != current_player->type ){	//Checking is this property owned by this user or not
		printf("Unfortunately, This property is not yours and you can't build a house here..\n");
	}
	else{
		while(front->house_count<3 && answer!=2){
		  printf("Do you want to build a house in this property? 1. YES 2. NO\n");
		  scanf("%d", &answer);
		 if(answer==1){
	      	if(current_player->account < front->house_price){	//Checking is money of the user enough for buying or not.. 
	      		printf("Unfortunately, you don't have enought money to build a new house! Please try later..\n");
	      		answer=2;
	      	}
	      	else{
	      		front->house_count++;	//increasing house count when buying..
	      		current_player->account=current_player->account - front->house_price;
	      		printf("Account after buying a house %d$\n", current_player->account);

	      	}
		 }
		 else{
		  printf("Going back to menu..\n");
		 }
		 
		}
	}
	printf("\n");

}

void sell_property_user(struct block *board, struct player* current_player){
	int i=0,j;
	int sell_p, temp, answer;
	int copy_id;
	struct block *front=board;
	if(current_player->owned_block_ids[0]==-1){
		printf("Please buy a property, before selling.\n");
	}
	else{
		printf("Please remember that the bank will pay only part of the price :(\n");
		printf("Properties owned by you.\n");
		printf("Please enter the number for selling..\n");
		while(current_player->owned_block_ids[i]!=-1){	//For  printing properties owned by me
			printf("%d. Block's ID - %d  \n", i+1, current_player->owned_block_ids[i]);
			i++;
	}
	scanf("%d", &sell_p);
		if(sell_p<1 || sell_p>12){	//If input is definitely incorrect, warn the user.
			printf("Incorrect input, please check it again\n");
		}
		else{	
			copy_id = current_player->owned_block_ids[sell_p-1];

			for(j=0; j<copy_id; j++){	
				board=board->next;
			}
			board->player_owner.type=noone;

			if(board->house_count==0) current_player->account+=board->price/2;
			if(board->house_count==1) current_player->account+=(board->price+board->house_price)/2;                
	        if(board->house_count==2) current_player->account+=(board->price+(2*board->house_price))/2;                          
	        if(board->house_count==3) current_player->account+=(board->price+(3*board->house_price))/2;
	   

			//I have to put -1 to the index of an owned array, because it is sold. And it is not our anymore.
			if(current_player->owned_block_ids[sell_p]==-1){	//If the next element of an array is already -1, it means we don't need to make any shifting after putting -1 (selling the property)
				current_player->owned_block_ids[sell_p-1]=-1;	//I made sell- 1, because it will help me to manage my owned_blocks array 
			}
			//ELSE IF below -> maded for shifting my array to the left by one.
			//I'm changing the property's ID which is solded by -1. To make it as an 'empty'
			if(current_player->owned_block_ids[sell_p]!=-1){	//If the next element is not -1, it means I have to shift by 1 element to the left. When I plug my -1 value to the array.
				current_player->owned_block_ids[sell_p-1]=-1;	
				temp=current_player->owned_block_ids[sell_p-1];

			    for(i=sell_p-1; i<12 && current_player->owned_block_ids[((sell_p-1)+1)-1]!=-1; i++){
			        if(current_player->owned_block_ids[(sell_p-1)+1]!=-1){
			        current_player->owned_block_ids[sell_p-1]=current_player->owned_block_ids[(sell_p-1)+1];
			        }
			        else{
			         current_player->owned_block_ids[((sell_p-1)+1)-1]=temp;
			        }

			        printf("My Owned array %d\n", current_player->owned_block_ids[i]);
			    }
			}
			printf("Account after selling -> %d$\n", current_player->account);
		}
	}
	board=front;
}

void after_dice_menu_user(struct block *board, struct player* current_player,struct player* player_one){
	int i, dice;
    int selection,insuf_flag=1, ans;
    struct block *front=board;
    for ( i = 0; i < current_player->current_block_id; i++) {
    	front=front->next;
    }
    switch (front->type)
    {
        case property:
            
            if(front->player_owner.type==noone && front->type==property){
				printf("%s has arrived %s \n",current_player->name,front->name);
				printf("This property is UNOWNED. \nDo you want to buy this property? 1. YES 2. NO\n");
				scanf("%d", &ans);

				if(ans==1){		
					
					buy_property_user(board, current_player);
				
					if (current_player->type==front->player_owner.type)
                    {
                        house_dialog(board,current_player);
                    }
				}	
			}
			else if(front->player_owner.type==cap){
				pay_rent_user(board,current_player,player_one);
			}
			printf("\e[5;31;47mPLAYER -> %s\e[0m\n", player_one->name);
         
            break;
        case tax:
            printf("%s has arrived tax block(%s)\n",current_player->name,front->name);
            while (count_property(*current_player)!=0 && current_player->account < front->rent) {
                insuf_flag = need_money(board, current_player, front->rent);
                printf("33333333333333333333333333333333333\n");
                if (insuf_flag == 0 && current_player->account < front->rent)
                    printf("You must pay your tax!\n");
            }
            printf("22222222222222222222222222\n");
            if (current_player->account>front->rent)
                current_player->account=current_player->account-front->rent;
            else
            {
                current_player->account=0;
            }
            break;
        case punish:
            printf("%s has punished for %d turn. \n",current_player->name,front->price);
            current_player->turn_to_wait+=front->price;
            break;

         case fortune:

			fortune_user(board, current_player, player_one);
         	break;
         case start:
         break;
    }
}

void pay_rent_user(struct block *board,struct player* current_player,struct player* owner){
    int rent=0,owned_property_count, i;
    for ( i = 0; i < current_player->current_block_id; i++) {
    	board=board->next;
    }
    rent=board->rent;
    if (board->house_count==1)rent=board->rent_1;
    if (board->house_count==2)rent=board->rent_2;
    if (board->house_count==3)rent=board->rent_3;
    if (rent>current_player->account)
    {
        owned_property_count=count_property(*current_player);
        while(rent>current_player->account && owned_property_count!=0)
        {
            need_money(board,current_player,rent);
            owned_property_count=count_property(*current_player);
        }
    }
    if (rent<current_player->account)
    {
        current_player->account-=rent;
        owner->account+=rent;
    } else
        current_player->account=0;
}

int count_property(struct player current_player){
    int i,count=0;
    for (i=0;i<12;i++)
    {
        if (current_player.owned_block_ids[i]!=-1)
            count++;
    }
    return count;
}

int need_money(struct block *board,struct player* current_player,int required_money){
    int selection,flag=1;
    while (selection!=4 && current_player->account < required_money && count_property(*current_player)!=0)
    {
        printf("You don't have enough money to continue. You need %d$ more. Please select an option to continue: \n",required_money-current_player->account);
        printf("1- Sell Property\n");
        printf("2- Show my Properties\n");
        printf("3- Show my Account\n");
        printf("0- Back\n");
        scanf("%d",&selection);
        switch (selection)
        {
            case 1:
                sell_property_user(board, current_player);
                break;
            case 2:
                show_my_properties(board, *current_player);
                break;
            case 3:
                printf("\n\nThere are %d $ in your account.\n\n",current_player->account);
                break;
            case 0:
                flag=0;
                break;
            default:
                printf("You made a wrong selection!\n");
                break;

        }
    }
    return flag;

}

	//Current player here, should be AI
void buy_property_AI(struct player* current_player, struct block *board){
	int i, j, flag=0;
	int result, m;
	for(m=0;m<current_player->current_block_id;m++){
		board=board->next;
	}
	result=check_property_AI(*current_player,board);

	if(result==1){
		for(j=0; j<12 && flag==0; j++){
			if(current_player->owned_block_ids[j]==-1){
			current_player->owned_block_ids[j]=board->block_id;				//Declaring this block into owned array
			current_player->account=current_player->account - board->price;	//Substacting money of player.
			board->player_owner=*current_player;	
				printf("New account of AI -> %d$\n", current_player->account);

			flag=1;
			}
			else if(j==11 && flag==0){	//I don't think this condition will be satisfied, but however I guess it is better to have it
				printf("All properties are owned! \n");
			}
		}
	}
	else if(result==0){
		printf("Next player's turn\n");
	}
}

void buy_house_AI(struct block* board, struct player* current_player){
	int i, count, dice, m;
	count=count_property(*current_player);
	for(m=0;m<current_player->current_block_id;m++){
		board=board->next;
	}
	
	if(count>=4){		//4, because 1/3 of all properues we have is - four
		dice = 1 + (rand() % 6);
		if(dice>=1 && dice <=3){
			if (board->house_count<3){
				if(board->house_price<=current_player->account){
					board->house_count+=1;	//increasing house count when buying..
					current_player->account=current_player->account - board->house_price;
					printf("Account after buying a house AI%d$\n", current_player->account);
				}
				else{
					printf("Next player's turn\n");
				}
       		}
       		else{
       		printf("Next player's turn\n");
       		}       
		}
		else if(dice>=4 && dice<=6){
			printf("Next player's turn\n");
		}
	}
	else{
		printf("Next player's turn\n");
	}
}

void pay_tax_AI(struct block *board, struct player* current_player){
	int tax=0, i, count; 
    struct block *front=board;
	for(i=0; i<current_player->current_block_id; i++){
		front=front->next;
	}
	tax=front->rent;
	if (tax > current_player->account){
		while(tax>current_player->account && count!=0)
        {
        	sell_property_AI(board, current_player);
            count=count_property(*current_player);
        }
	}
	if (tax<current_player->account){
        current_player->account-=tax;  
    }
     else
        current_player->account=0;

}

void pay_rent_AI(struct block *board, struct player* current_player,struct player* player_two){
	int rent=0, i, count;
	struct block *front=board;
	for(i=0; i<current_player->current_block_id; i++){
		front=front->next;
	}
	rent=front->rent;	
	if (front->house_count==1) rent=front->rent_1;
    if (front->house_count==2) rent=front->rent_2;
    if (front->house_count==3) rent=front->rent_3;

    if (rent>current_player->account){

    	 while(rent>current_player->account && count!=0)
        {
        	sell_property_AI(board, current_player);
            count=count_property(*current_player);
        }
    }   
    if (rent<current_player->account){
        current_player->account-=rent;
        front->player_owner.account = front->player_owner.account + rent;   
    }
    else
        current_player->account=0;
}
void fortune_AI(struct block *board,struct player* current_player, int dice, struct player* player_two){
	int i=0,j=1, found=0, turn;
	struct block * front;
	front=board;
	
	switch(dice){
		case 1:
			if(current_player->owned_block_ids[0]==-1){
				turn=2;
			}			
			else{
				for (i = 0; i < current_player->owned_block_ids[0]; i++){
					front=front->next;
				}
				if(front->house_count<3){
					front->house_count+=1;	//increasing house count for free.
					found=1;
				}					//If, in the first owned property AI already have 3 houses, we will enter the 'while' loop.
				while(found!=1){	//Until we will find suitable property for building a house there.
					front=front;
					for(i=0; i<current_player->owned_block_ids[j] && current_player->owned_block_ids[j]!=-1; i++){
						front=front->next;			
					}
					if(front->house_count<3){
						front->house_count+=1;
						found=1;
					}
					j++;
				}
			}
			break;
		case 2:
			dice = 1 + (rand() % 6);
			if(dice>=1 && dice <=3){
				current_player->current_block_id=current_player->current_block_id+2;
			}
			else if(dice>=4 && dice<=6){
				current_player->current_block_id=current_player->current_block_id-2;
			}
			break;
		case 3:
			current_player->account=current_player->account-5000;
			break;
		case 4:
			current_player->account=current_player->account-10000;
			player_two->account=player_two->account+10000;
			break;
		case 5:	
			current_player->account=current_player->account+20000;
			break;
	}

}

void house_dialog(struct block* board, struct player* current_player){
    int house_flag=1,selection,insuf_flag=1, i;
    struct block* front=board;
    for (i = 0; i < current_player->current_block_id; i++){
   		front=front->next;
	}
    while (house_flag && selection==1) {
        printf("Do you want to build house on %s ?\n",front->name);
        printf("1- Yes\n");
        printf("2- No\n");
        scanf("%d", &selection);
        if (selection == 1) {

            if (current_player->account < front->house_price) {
                while (count_property(*current_player)!=0 && current_player->account < front->house_price && insuf_flag==1)
                    insuf_flag=need_money(board,current_player,front->house_price);
            }

            if (current_player->account >= front->house_price){
                buy_house_user(board, current_player);
            }
            else{
                house_flag=0;
            }

        } else{
            house_flag=0;
        }
    }
}

void fortune_user(struct block *board,struct player* current_player, struct player* player_one){
	int i=0,j=1, found=0, dice;
	struct block * front;
	front=board;

	dice = 1 + (rand() % 5);
	
	switch(dice){
		case 1:
			if(current_player->owned_block_ids[0]==-1){
				printf("Next player's turn\n");
			}			
			else{
				for (i = 0; i < current_player->owned_block_ids[0]; i++){
					front=front->next;
				}
				if(front->house_count<3){
					front->house_count+=1;	//increasing house count for free.
					found=1;
				}					//If, in the first owned property AI already have 3 houses, we will enter the 'while' loop.
				while(found!=1){	//Until we will find suitable property for building a house there.
					front=board;
					for(i=0; i<current_player->owned_block_ids[j] && current_player->owned_block_ids[j]!=-1; i++){
						front=front->next;			
					}
					if(front->house_count<3){
						front->house_count+=1;
						found=1;
					}
					j++;
				}
			}
			break;
		case 2:
			dice = 1 + (rand() % 6);
			if(dice>=1 && dice <=3){
				current_player->current_block_id=current_player->current_block_id+2;
			}
			else if(dice>=4 && dice<=6){
				current_player->current_block_id=current_player->current_block_id-2;
			}
			break;
		case 3:
			current_player->account=current_player->account-5000;
			break;
		case 4:
			current_player->account=current_player->account-10000;
			player_one->account=player_one->account+10000;
			break;
		case 5:	
			current_player->account=current_player->account+20000;
			break;
	}

}

void sell_property_AI(struct block *board, struct player* current_player){
	int max, i=1, j, k, copy_max;
	struct block *front;	//start shows beginning of the linked list
	struct block *temp;
	int value;
	int delete, n=0;

	front=board;

		copy_max=current_player->owned_block_ids[0];

		for(i=0; i<copy_max; i++){
			front=front->next;
		}
		max=front->price;		//first one

		while(current_player->owned_block_ids[i]!=-1){
			front=front;
			for(j=0; j<current_player->owned_block_ids[i] && current_player->owned_block_ids[i]!=-1; j++){
				front=board;
				for(k=0; k<current_player->owned_block_ids[j]; k++){
					front=front->next;
				}
				if (front->price > max){
			       max = front->price;
			       temp=front; 	//For not loosing board with max price
				}		
			}
				printf("MAX priece of property AI%d\n", max);	
			i++;
		}

		front=temp;
		delete=front->block_id;
		front->player_owner.type=noone;

		if(front->house_count==0) current_player->account+=front->price/2;
		if(front->house_count==1) current_player->account+=(front->price+front->house_price)/2;                
        if(front->house_count==2)  current_player->account+=(front->price+(2*front->house_price))/2;                          
	    if(front->house_count==3)  current_player->account+=(front->price+(3*front->house_price))/2;

	   
	    while(current_player->owned_block_ids[n]!=-1){
	    	if(delete==current_player->owned_block_ids[n]){
	    		 //I have to put -1 to the index of an owned array, because it is sold. And it is not our anymore.
				if(current_player->owned_block_ids[n+1]==-1){	//If the next element of an array is already -1, it means we don't need to make any shifting after putting -1 (selling the property)
					current_player->owned_block_ids[n]=-1;	//I made sell- 1, because it will help me to manage my owned_blocks array 
				}
				//ELSE IF below -> maded for shifting my array to the left by one.
				//I'm changing the property's ID which is solded by -1. To make it as an 'empty'
				if(current_player->owned_block_ids[n+1]!=-1){	//If the next element is not -1, it means I have to shift by 1 element to the left. When I plug my -1 value to the array.
					current_player->owned_block_ids[n]=-1;	
					value=current_player->owned_block_ids[n];

				    for(i=n; i<12 && current_player->owned_block_ids[((n)+1)-1]!=-1; i++){
				        if(current_player->owned_block_ids[(n)+1]!=-1){
				        current_player->owned_block_ids[n]=current_player->owned_block_ids[(n)+1];
				        }
				        else{
				         current_player->owned_block_ids[((n)+1)-1]=value;
				        }

				        printf("My Owned array AI AI %d\n", current_player->owned_block_ids[n]);
				    }
				}
			}
	    	n++;
	    }

	   
		printf("Account of AI after selling -> %d$\n", current_player->account);

	
	
}

//I will send AI for checking all probabilities for buying a house, property etc.
int check_property_AI( struct player current_player, struct block *board){
	int result, avg, dice, i;
	struct block *front;
	front=board;
	for(i=0;i<current_player.current_block_id;i++){
		front=front->next;
	}

		avg=sum_avg(board);
		if(front->price < avg){
			result=1;					//Result=1 means buy.
			printf("Average is  %d\n", avg);
		}
		else{
			dice = 1 + (rand() % 6);
			printf("Average cost was not bigger than price!!\n");
			printf("Computer rolled the dice: N %d\n", dice);

			if(dice>=1 && dice<=3){
				result=1; 				//Buy it
			}
			else if(dice>=4 && dice<=6){
				result=0;           	//skip the turn.
			}
		}
return result;
} 


int sum_avg(struct block *board){
	int sum=0, avg;
	struct block *front;
	front=board;

	while(front!=NULL){
		sum=sum+front->price;
		front=front->next;
	}
	avg=sum/12;	//Because, we have 12 properties
	return avg;
}










