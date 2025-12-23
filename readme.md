นี่คือเอกสารประกอบโครงงานฉบับสมบูรณ์สำหรับ **"ระบบบริหารจัดการสต็อกอะไหล่พร้อมระบบบันทึกประวัติ (Advanced Spare Parts Inventory with CRUD & Audit Log)"** ครับ

ผมได้รวมโค้ดเวอร์ชันล่าสุดที่ **แก้บั๊ก Infinite Loop (ลูปนรก)** และรวมฟีเจอร์ **CRUD + Log** ไว้ให้แล้ว สามารถนำเนื้อหานี้ไปลงเอกสารรายงานได้เลยครับ

---

# ชื่อโครงงาน: ระบบบริหารจัดการสต็อกอะไหล่และบันทึกประวัติการทำงาน (Spare Parts Inventory System with CRUD & Log)

## 1. ที่มาและความสำคัญของปัญหา

ในกระบวนการซ่อมบำรุงและผลิตในโรงงานอุตสาหกรรม การบริหารจัดการอะไหล่ (Spare Parts) มีความสำคัญอย่างยิ่ง ปัญหาที่พบบ่อยคือ "ข้อมูลไม่เป็นปัจจุบัน" และ "ไม่สามารถตรวจสอบย้อนหลังได้" ว่าใครเป็นคนเบิกของ หรือแก้ไขจำนวนเมื่อไหร่ การใช้เพียงการจดบันทึกใส่กระดาษทำให้เกิดความผิดพลาด (Human Error) และเมื่อของหายหรือจำนวนไม่ตรง ก็ไม่สามารถหาสาเหตุได้ ระบบดิจิทัลที่สามารถเพิ่ม ลบ แก้ไข และบันทึกประวัติ (Log) จึงเป็นสิ่งจำเป็นเพื่อความโปร่งใสและแม่นยำ

## 2. วัตถุประสงค์ของการวิจัย

1. เพื่อพัฒนาโปรแกรมบริหารจัดการสต็อกที่รองรับฟังก์ชันพื้นฐานครบถ้วน (CRUD: Create, Read, Update, Delete)
2. เพื่อสร้างระบบบันทึกประวัติการทำงาน (Audit Trail/Log) สำหรับตรวจสอบความถูกต้องย้อนหลัง
3. เพื่อป้องกันปัญหาข้อมูลผิดพลาดจากการป้อนข้อมูลของผู้ใช้งาน (Input Validation)

## 3. ประโยชน์ที่คาดว่าจะได้รับ

1. สามารถตรวจสอบจำนวนสินค้าคงเหลือได้อย่างรวดเร็วและแม่นยำ
2. มีหลักฐาน (Log) ยืนยันทุกธุรกรรมการเบิกจ่ายหรือแก้ไขข้อมูล
3. ลดความผิดพลาดจากการทำงานซ้ำซ้อนและการสูญหายของข้อมูล

## 4. ขอบเขตของงานวิจัย

* **ภาษาที่ใช้:** C Programming Language (Standard C)
* **เครื่องมือพัฒนา:** OnlineGDB Compiler
* **ฟังก์ชันหลัก:**
* **C**reate: เพิ่มรายชื่ออะไหล่ใหม่ (Auto ID)
* **R**ead: แสดงรายการอะไหล่ และ แสดงประวัติการทำงาน (Log History)
* **U**pdate: แก้ไขรายละเอียดอะไหล่ และ ตัดสต็อก (Withdraw)
* **D**elete: ลบรายชื่ออะไหล่ออกจากระบบ


* **ข้อจำกัด:** ข้อมูลจะถูกเก็บในหน่วยความจำชั่วคราว (RAM Array) ไม่มีการบันทึกเป็นไฟล์ถาวรในเวอร์ชันนี้

## 5. ขั้นตอนการดำเนินงาน

1. **Requirement Analysis:** วิเคราะห์ความต้องการระบบ CRUD และ Log
2. **Design:** ออกแบบโครงสร้างข้อมูล (Struct) สำหรับสินค้า และ Array สำหรับเก็บข้อความ Log
3. **Implementation:** เขียนโค้ดภาษา C โดยแบ่งเป็นฟังก์ชันย่อย และใส่ระบบป้องกัน Input Error
4. **Testing:** ทดสอบการ เพิ่ม ลบ แก้ไข และดู Log ว่าบันทึกถูกต้องตามเวลาจริงหรือไม่
5. **Documentation:** จัดทำเอกสารและคู่มือการใช้งาน

---

## 6. ผังงาน (Flowchart)

```mermaid
graph TD
    Start([Start]) --> Init[Init Dummy Data & Log]
    Init --> Menu{Main Menu}
    
    Menu -->|1. Create| Add[Input Details -> Save to Struct]
    Add --> Log1[Record Log: Added ID...]
    
    Menu -->|2. Read Stock| Show[Loop & Print Inventory Array]
    
    Menu -->|3. Update/Edit| Edit[Search ID -> Edit Name/Min]
    Edit --> Log2[Record Log: Edited ID...]
    
    Menu -->|4. Withdraw| Draw[Search ID -> Decrease Qty]
    Draw --> CheckMin{Qty <= Min?}
    CheckMin -- Yes --> Alert[Show ALERT!]
    CheckMin -- No --> Log3
    Alert --> Log3[Record Log: Withdraw & Alert]
    
    Menu -->|5. Delete| Del[Search ID -> Shift Array Left]
    Del --> Log4[Record Log: Deleted ID...]
    
    Menu -->|6. View Log| Hist[Loop & Print Log Array]
    
    Log1 --> Menu
    Log2 --> Menu
    Log3 --> Menu
    Log4 --> Menu
    Show --> Menu
    Hist --> Menu
    
    Menu -->|0. Exit| End([End])

```

---

## 7. การเขียนโค้ด (Source Code)

*หมายเหตุ: โค้ดนี้รวมการแก้ปัญหา Input Error (ลูปนรก) ไว้ในฟังก์ชัน `main` แล้ว*

```c
#include <stdio.h>
#include <string.h>
#include <time.h> // ใช้สำหรับดึงเวลาปัจจุบัน

// --- 1. โครงสร้างข้อมูล (Data Structures) ---
struct Part {
    int id;
    char name[50];
    int quantity;
    int min_level;
};

// Global Variables (ตัวแปรที่ใช้ร่วมกันทั้งโปรแกรม)
struct Part inventory[100];     // เก็บสินค้าสูงสุด 100 รายการ
int count = 0;                  // ตัวนับจำนวนสินค้าปัจจุบัน
char transactionLog[100][150];  // เก็บประวัติ 100 บรรทัด
int logCount = 0;               // ตัวนับจำนวน Log

// --- 2. ฟังก์ชันช่วยงาน (Helper Functions) ---

// ฟังก์ชันบันทึก Log พร้อมเวลาปัจจุบัน
void recordLog(char *action) {
    time_t now;
    time(&now);
    struct tm *local = localtime(&now);
    char time_str[30];
    
    // แปลงเวลาเป็น format: YYYY-MM-DD HH:MM:SS
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", local);

    // บันทึกลง Array: [เวลา] การกระทำ
    sprintf(transactionLog[logCount], "[%s] %s", time_str, action);
    logCount++;
    
    // (Optional) ถ้า Log เต็ม ให้ reset หรือขยับ (ในที่นี้ปล่อยไว้ก่อนเพื่อความง่าย)
}

// --- 3. ฟังก์ชัน CRUD (Create, Read, Update, Delete) ---

// [C]reate: เพิ่มสินค้า
void addPart() {
    char logMsg[100];
    printf("\n--- [C] Create New Part ---\n");
    
    // สร้าง ID อัตโนมัติ (Auto Increment)
    int newID = (count == 0) ? 1 : inventory[count-1].id + 1;
    inventory[count].id = newID;
    
    printf("Enter Part Name: ");
    scanf("%s", inventory[count].name);
    printf("Enter Quantity: ");
    scanf("%d", &inventory[count].quantity);
    printf("Enter Min Safety Level: ");
    scanf("%d", &inventory[count].min_level);
    
    printf(">> Success! Added ID: %d\n", newID);
    
    // บันทึก Log
    sprintf(logMsg, "CREATED Part ID %d (%s), Qty: %d", newID, inventory[count].name, inventory[count].quantity);
    recordLog(logMsg);
    
    count++;
}

// [R]ead: แสดงสินค้าทั้งหมด
void displayInventory() {
    printf("\n--- [R] Current Stock Table ---\n");
    printf("ID\tName\t\tQty\tMin\tStatus\n");
    printf("----------------------------------------------------\n");
    for(int i = 0; i < count; i++) {
        printf("%d\t%-10s\t%d\t%d\t", inventory[i].id, inventory[i].name, inventory[i].quantity, inventory[i].min_level);
        if(inventory[i].quantity <= inventory[i].min_level) printf("[LOW STOCK!]");
        else printf("OK");
        printf("\n");
    }
    printf("----------------------------------------------------\n");
}

// [R]ead Log: แสดงประวัติการทำงาน
void viewLogHistory() {
    printf("\n--- [Log] System Audit Trail ---\n");
    if(logCount == 0) {
        printf("No history found.\n");
    } else {
        for(int i = 0; i < logCount; i++) {
            printf("%s\n", transactionLog[i]);
        }
    }
    printf("--------------------------------------------\n");
}

// [U]pdate: แก้ไขข้อมูล
void editPart() {
    int id, found = 0;
    char logMsg[100];
    
    printf("\n--- [U] Edit Part Details ---\n");
    printf("Enter ID to edit: ");
    if(scanf("%d", &id) != 1) { while(getchar() != '\n'); return; } // กัน Error
    
    for(int i = 0; i < count; i++) {
        if(inventory[i].id == id) {
            found = 1;
            printf("Editing ID %d (%s)...\n", id, inventory[i].name);
            printf("Enter New Name: ");
            scanf("%s", inventory[i].name);
            printf("Enter New Min Level: ");
            scanf("%d", &inventory[i].min_level);
            
            printf(">> Update Successful!\n");
            
            sprintf(logMsg, "UPDATED Part ID %d to Name: %s, Min: %d", id, inventory[i].name, inventory[i].min_level);
            recordLog(logMsg);
            break;
        }
    }
    if(!found) printf("Error: ID not found.\n");
}

// [U]pdate (Transaction): เบิกสินค้า
void withdrawPart() {
    int id, qty, found = 0;
    char logMsg[100];
    
    printf("\n--- Withdraw Stock ---\n");
    printf("Enter ID: ");
    if(scanf("%d", &id) != 1) { while(getchar() != '\n'); return; }
    
    for(int i = 0; i < count; i++) {
        if(inventory[i].id == id) {
            found = 1;
            printf("Current Stock: %d. Withdraw Amount: ", inventory[i].quantity);
            scanf("%d", &qty);
            
            if(qty > inventory[i].quantity) {
                printf("Error: Not enough stock!\n");
            } else {
                inventory[i].quantity -= qty;
                printf(">> Withdraw Done. Remaining: %d\n", inventory[i].quantity);
                
                sprintf(logMsg, "WITHDRAW ID %d (%s) -%d pcs. Bal: %d", id, inventory[i].name, qty, inventory[i].quantity);
                recordLog(logMsg);

                // เช็คจุดสั่งซื้อ
                if(inventory[i].quantity <= inventory[i].min_level) {
                    printf("\n [!!! WARNING !!!] Stock is below Min Level!\n");
                    sprintf(logMsg, "ALERT! Low Stock on ID %d", id);
                    recordLog(logMsg);
                }
            }
            break;
        }
    }
    if(!found) printf("Error: ID not found.\n");
}

// [D]elete: ลบสินค้า
void deletePart() {
    int id, found = 0;
    char logMsg[100];
    
    printf("\n--- [D] Delete Part ---\n");
    printf("Enter ID to DELETE: ");
    if(scanf("%d", &id) != 1) { while(getchar() != '\n'); return; }
    
    for(int i = 0; i < count; i++) {
        if(inventory[i].id == id) {
            found = 1;
            char deletedName[50];
            strcpy(deletedName, inventory[i].name);
            
            // Shift ข้อมูลตัวหลังมาทับตัวหน้า
            for(int j = i; j < count - 1; j++) {
                inventory[j] = inventory[j+1];
            }
            count--; // ลดจำนวนสินค้า
            
            printf(">> Deleted ID %d successfully.\n", id);
            
            sprintf(logMsg, "DELETED Part ID %d (%s)", id, deletedName);
            recordLog(logMsg);
            break;
        }
    }
    if(!found) printf("Error: ID not found.\n");
}

// --- Main Program ---
int main() {
    int choice;
    
    // สร้างข้อมูลจำลอง (Dummy Data)
    if(count == 0) {
        inventory[0].id = 1; strcpy(inventory[0].name, "Motor_X1"); inventory[0].quantity = 10; inventory[0].min_level = 2; count++;
        recordLog("SYSTEM STARTED. Initialized Default Data.");
    }

    do {
        printf("\n========== INVENTORY SYSTEM V2 (CRUD + LOG) ==========\n");
        printf("1. [C] Create New Part\n");
        printf("2. [R] Show All Stock\n");
        printf("3. [U] Edit Part Details\n");
        printf("4. [U] Withdraw Part (Transaction)\n");
        printf("5. [D] Delete Part\n");
        printf("6. [L] View History Log\n");
        printf("0. Exit\n");
        printf("Select: ");
        
        // --- ส่วนป้องกัน Infinite Loop (Input Validation) ---
        if (scanf("%d", &choice) != 1) {
            // หากผู้ใช้พิมพ์ตัวอักษรแทนตัวเลข ให้เคลียร์ค่าทิ้ง
            while(getchar() != '\n'); 
            choice = -1; // บังคับให้เข้า default case
        }
        // ------------------------------------------------

        switch(choice) {
            case 1: addPart(); break;
            case 2: displayInventory(); break;
            case 3: editPart(); break;
            case 4: withdrawPart(); break;
            case 5: deletePart(); break;
            case 6: viewLogHistory(); break;
            case 0: printf("Exiting program...\n"); break;
            default: printf("Invalid choice! Please enter number 0-6.\n");
        }
    } while (choice != 0);

    return 0;
}

```

---

## 8. สรุปผล (Conclusion)

โครงงานนี้ประสบความสำเร็จในการพัฒนาระบบบริหารจัดการสต็อกอะไหล่ โดยมีความสามารถครอบคลุมตามวัตถุประสงค์ ดังนี้:

1. **CRUD Complete:** สามารถจัดการข้อมูล (เพิ่ม/ลบ/แก้ไข) ได้อย่างถูกต้อง โดยมีการจัดการลำดับของ Array เมื่อมีการลบข้อมูลได้อย่างมีประสิทธิภาพ
2. **Audit Trail:** ระบบ Log สามารถบันทึกเวลาและกิจกรรมที่เกิดขึ้นจริง ช่วยให้ผู้ดูแลระบบสามารถตรวจสอบย้อนหลังได้ว่าเกิดอะไรขึ้นกับสินค้าชิ้นไหน
3. **Reliability:** มีการเพิ่มระบบดักจับข้อผิดพลาด (Input Validation) ทำให้โปรแกรมไม่ค้างเมื่อผู้ใช้กรอกข้อมูลผิดรูปแบบ

## 9. ข้อเสนอแนะ (Recommendations)

1. **File Database:** พัฒนาต่อโดยใช้ File I/O (fprintf, fscanf) เพื่อให้บันทึกข้อมูลและ Log ลงไฟล์ .txt หรือ .csv ได้อย่างถาวร (ข้อมูลไม่หายเมื่อปิดโปรแกรม)
2. **User Authentication:** เพิ่มระบบ Login (Username/Password) เพื่อระบุตัวตนใน Log ได้ชัดเจนขึ้นว่า "ใคร" เป็นคนทำรายการ
3. **Search Function:** หากข้อมูลมีจำนวนมาก ควรเพิ่มฟังก์ชันค้นหาด้วยชื่อ (Search by Name) นอกเหนือจากการค้นหาด้วย ID