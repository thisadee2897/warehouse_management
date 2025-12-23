#include <stdio.h>
#include <string.h>
#include <time.h> // ใช้สำหรับดึงเวลาปัจจุบัน

// --- 1. ตั้งค่าสีและรูปแบบ (ANSI Colors) ---
#define COLOR_RESET   "\x1b[0m"
#define COLOR_RED     "\x1b[31m"     // สีแดง (ลบ/แจ้งเตือน)
#define COLOR_GREEN   "\x1b[32m"     // สีเขียว (เพิ่ม)
#define COLOR_YELLOW  "\x1b[33m"     // สีเหลือง (แก้ไข)
#define COLOR_BLUE    "\x1b[34m"     // สีน้ำเงิน (Header)
#define COLOR_CYAN    "\x1b[36m"     // สีฟ้า (Info)
#define COLOR_BOLD    "\x1b[1m"      // ตัวหนา

// --- 2. โครงสร้างข้อมูล (Data Structures) ---
struct Part {
    int id;
    char name[50];
    int quantity;
    int min_level;
};

// Global Variables
struct Part inventory[100];     // เก็บสินค้าสูงสุด 100 รายการ
int count = 0;                  // ตัวนับสินค้า
char transactionLog[100][200];  // เก็บประวัติ (ขยายขนาดบรรทัดเพื่อให้ใส่ Code สีได้พอ)
int logCount = 0;               // ตัวนับประวัติ

// --- 3. Helper Functions (ฟังก์ชันช่วยงาน) ---

// ฟังก์ชันบันทึก Log แบบสวยงาม (Auto-Format Table)
void recordLog(char *action, char *type) {
    time_t now;
    time(&now);
    struct tm *local = localtime(&now);
    char time_str[30];
    
    // ดึงเวลาปัจจุบัน Format: YYYY-MM-DD HH:MM:SS
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", local);

    // เลือกสีตามประเภท (Type)
    char color[15];
    char typeIcon[10]; 
    
    if (strcmp(type, "ADD") == 0) { 
        strcpy(color, COLOR_GREEN); strcpy(typeIcon, "[+]"); 
    } else if (strcmp(type, "DEL") == 0) { 
        strcpy(color, COLOR_RED); strcpy(typeIcon, "[-]");
    } else if (strcmp(type, "EDIT") == 0) { 
        strcpy(color, COLOR_YELLOW); strcpy(typeIcon, "[*]");
    } else if (strcmp(type, "ALERT") == 0) { 
        strcpy(color, COLOR_RED); strcpy(typeIcon, "[!]");
    } else { 
        strcpy(color, COLOR_CYAN); strcpy(typeIcon, "[i]");
    }

    // จัด Format ลงในตาราง: | TIMESTAMP | TYPE | DETAILS |
    sprintf(transactionLog[logCount], 
            "| %s | %s%s %-6s %s | %-55s |", 
            time_str, color, COLOR_BOLD, type, COLOR_RESET, action);
            
    logCount++;
}

// ฟังก์ชันวาดเส้นคั่นตาราง (เพื่อความสวยงามและเรียกใช้ง่าย)
void printLine(int width) {
    printf("+");
    for(int i=0; i<width; i++) printf("-");
    printf("+\n");
}

// --- 4. CRUD Functions ---

// [C]reate: เพิ่มสินค้า
void addPart() {
    char logMsg[100];
    printf("\n%s--- [ Create New Part ] ---%s\n", COLOR_GREEN, COLOR_RESET);
    
    int newID = (count == 0) ? 1 : inventory[count-1].id + 1; // Auto ID
    inventory[count].id = newID;
    
    printf(" Enter Part Name      : ");
    scanf("%s", inventory[count].name);
    printf(" Enter Quantity       : ");
    scanf("%d", &inventory[count].quantity);
    printf(" Enter Min Safety Lvl : ");
    scanf("%d", &inventory[count].min_level);
    
    printf("%s >> Success! Added ID: %d%s\n", COLOR_GREEN, newID, COLOR_RESET);
    
    sprintf(logMsg, "Added '%s' (Qty: %d, Min: %d)", inventory[count].name, inventory[count].quantity, inventory[count].min_level);
    recordLog(logMsg, "ADD"); // ส่ง Type ADD
    
    count++;
}

// [R]ead: แสดงสินค้า (ตารางสวยงาม)
void displayInventory() {
    printf("\n%s--- Current Inventory Stock ---%s\n", COLOR_BLUE, COLOR_RESET);
    
    // Header Table
    printf("+-----+----------------------+----------+----------+----------------+\n");
    printf("| %-3s | %-20s | %-8s | %-8s | %-14s |\n", "ID", "PART NAME", "QTY", "MIN", "STATUS");
    printf("+-----+----------------------+----------+----------+----------------+\n");
    
    for(int i = 0; i < count; i++) {
        char status[50];
        
        // Check Status logic
        if(inventory[i].quantity <= inventory[i].min_level) {
            sprintf(status, "%s%sLOW STOCK!%s", COLOR_RED, COLOR_BOLD, COLOR_RESET);
        } else {
            sprintf(status, "%sOK%s", COLOR_GREEN, COLOR_RESET);
        }
        
        // Print Row
        // หมายเหตุ: %-20s คือจองพื้นที่ 20 ตัวอักษรและชิดซ้าย
        printf("| %-3d | %-20s | %-8d | %-8d | %-23s |\n", 
               inventory[i].id, inventory[i].name, inventory[i].quantity, inventory[i].min_level, status);
    }
    printf("+-----+----------------------+----------+----------+----------------+\n");
}

// [R]ead Log: แสดงประวัติ (ตารางสวยงาม)
void viewLogHistory() {
    printf("\n%s--- System Audit Trail (Log) ---%s\n", COLOR_CYAN, COLOR_RESET);
    
    printf("+---------------------+------------+---------------------------------------------------------+\n");
    printf("| %-19s | %-10s | %-55s |\n", "TIMESTAMP", "TYPE", "ACTION DETAILS");
    printf("+---------------------+------------+---------------------------------------------------------+\n");
    
    if(logCount == 0) {
        printf("| %-88s |\n", " No history found.");
    } else {
        for(int i = 0; i < logCount; i++) {
            printf("%s\n", transactionLog[i]);
        }
    }
    printf("+---------------------+------------+---------------------------------------------------------+\n");
}

// [U]pdate: แก้ไขข้อมูล
void editPart() {
    int id, found = 0;
    char logMsg[100];
    
    printf("\n%s--- [ Edit Part Details ] ---%s\n", COLOR_YELLOW, COLOR_RESET);
    printf(" Enter ID to edit: ");
    if(scanf("%d", &id) != 1) { while(getchar() != '\n'); return; }
    
    for(int i = 0; i < count; i++) {
        if(inventory[i].id == id) {
            found = 1;
            printf(" Editing ID %d (%s)...\n", id, inventory[i].name);
            printf(" Enter New Name      : ");
            scanf("%s", inventory[i].name);
            printf(" Enter New Min Level : ");
            scanf("%d", &inventory[i].min_level);
            
            printf("%s >> Update Successful!%s\n", COLOR_YELLOW, COLOR_RESET);
            
            sprintf(logMsg, "Updated ID %d. New Name: %s, Min: %d", id, inventory[i].name, inventory[i].min_level);
            recordLog(logMsg, "EDIT");
            break;
        }
    }
    if(!found) printf(" %sError: ID not found.%s\n", COLOR_RED, COLOR_RESET);
}

// [U]pdate Transaction: เบิกของ
void withdrawPart() {
    int id, qty, found = 0;
    char logMsg[100];
    
    printf("\n%s--- [ Withdraw / Cut Stock ] ---%s\n", COLOR_BLUE, COLOR_RESET);
    printf(" Enter ID: ");
    if(scanf("%d", &id) != 1) { while(getchar() != '\n'); return; }
    
    for(int i = 0; i < count; i++) {
        if(inventory[i].id == id) {
            found = 1;
            printf(" Current: %d. Withdraw Amount: ", inventory[i].quantity);
            scanf("%d", &qty);
            
            if(qty > inventory[i].quantity) {
                printf(" %sError: Not enough stock!%s\n", COLOR_RED, COLOR_RESET);
            } else {
                inventory[i].quantity -= qty;
                printf("%s >> Withdraw Done. Remaining: %d%s\n", COLOR_GREEN, inventory[i].quantity, COLOR_RESET);
                
                sprintf(logMsg, "Withdraw ID %d (%s) -%d pcs. Bal: %d", id, inventory[i].name, qty, inventory[i].quantity);
                recordLog(logMsg, "WITHDRAW"); // ใช้ Type ทั่วไป หรือสร้างใหม่ก็ได้

                // Check Min Level
                if(inventory[i].quantity <= inventory[i].min_level) {
                    printf("\n%s [!!! WARNING !!!] Stock is below Min Level!%s\n", COLOR_RED, COLOR_RESET);
                    sprintf(logMsg, "ALERT! Low Stock on ID %d (%s)", id, inventory[i].name);
                    recordLog(logMsg, "ALERT"); // แจ้งเตือนสีแดง
                }
            }
            break;
        }
    }
    if(!found) printf(" %sError: ID not found.%s\n", COLOR_RED, COLOR_RESET);
}

// [D]elete: ลบสินค้า
void deletePart() {
    int id, found = 0;
    char logMsg[100];
    
    printf("\n%s--- [ Delete Part ] ---%s\n", COLOR_RED, COLOR_RESET);
    printf(" Enter ID to DELETE: ");
    if(scanf("%d", &id) != 1) { while(getchar() != '\n'); return; }
    
    for(int i = 0; i < count; i++) {
        if(inventory[i].id == id) {
            found = 1;
            char deletedName[50];
            strcpy(deletedName, inventory[i].name);
            
            // Shift Array
            for(int j = i; j < count - 1; j++) {
                inventory[j] = inventory[j+1];
            }
            count--; 
            
            printf("%s >> Deleted ID %d (%s) successfully.%s\n", COLOR_RED, id, deletedName, COLOR_RESET);
            
            sprintf(logMsg, "Deleted Part ID %d (%s)", id, deletedName);
            recordLog(logMsg, "DEL");
            break;
        }
    }
    if(!found) printf(" %sError: ID not found.%s\n", COLOR_RED, COLOR_RESET);
}

// --- Main Program ---
int main() {
    int choice;
    
    // Init Dummy Data
    if(count == 0) {
        inventory[0].id = 1; strcpy(inventory[0].name, "Bearing_6201"); inventory[0].quantity = 50; inventory[0].min_level = 10; count++;
        inventory[1].id = 2; strcpy(inventory[1].name, "Belt_Fan_A1"); inventory[1].quantity = 5; inventory[1].min_level = 15; count++;
        recordLog("SYSTEM STARTED. Initialized Dummy Data.", "INFO");
    }

    do {
        // Main Menu Display
        printf("\n");
        printf("%s==========================================%s\n", COLOR_BOLD, COLOR_RESET);
        printf("%s    FACTORY INVENTORY SYSTEM V3.0    %s\n", COLOR_CYAN, COLOR_RESET);
        printf("%s==========================================%s\n", COLOR_BOLD, COLOR_RESET);
        printf(" 1. %s[C]%s Create New Part\n", COLOR_GREEN, COLOR_RESET);
        printf(" 2. %s[R]%s Show All Stock (Table)\n", COLOR_BLUE, COLOR_RESET);
        printf(" 3. %s[U]%s Edit Part Details\n", COLOR_YELLOW, COLOR_RESET);
        printf(" 4. %s[U]%s Withdraw / Cut Stock\n", COLOR_YELLOW, COLOR_RESET);
        printf(" 5. %s[D]%s Delete Part\n", COLOR_RED, COLOR_RESET);
        printf(" 6. %s[L]%s View History Log\n", COLOR_CYAN, COLOR_RESET);
        printf(" 0. Exit\n");
        printf("%s------------------------------------------%s\n", COLOR_BOLD, COLOR_RESET);
        printf(" Select Menu: ");
        
        // Input Validation (กัน Infinite Loop)
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n'); 
            choice = -1; 
        }

        switch(choice) {
            case 1: addPart(); break;
            case 2: displayInventory(); break;
            case 3: editPart(); break;
            case 4: withdrawPart(); break;
            case 5: deletePart(); break;
            case 6: viewLogHistory(); break;
            case 0: printf(" Exiting program... Good Bye!\n"); break;
            default: printf(" %sInvalid choice! Please try again.%s\n", COLOR_RED, COLOR_RESET);
        }
    } while (choice != 0);

    return 0;
}