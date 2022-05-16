#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>

SoftwareSerial BLU(0, 1);

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
MFRC522::MIFARE_Key key;
MFRC522::StatusCode status;

int BLOCK = 1;
byte WRITE_DATA [16] = {"0000000000000000"};
byte bufferLen = 18;
byte READ_DATA[18];
int TOPUP = 0;
int CURRENT = 0;

void setup()
{
  Serial.begin(9600);   // Initiate a serial communication
  BLU.begin(9600);
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approach your card to the reader...");
  Serial.println();

}
void loop()
{
  // Receive top-up sum if available
  if (BLU.available() > 0) {
    Serial.println();
    Serial.println("Receiving top-up...");
    String string = "";
    while (BLU.available() > 0)
    {
      char command = ((byte)BLU.read());
      if (command == ';')
      {
        TOPUP = string.toInt();
        break;
      }
      else {
        string += command;
      }
      delay(1);
    }
    Serial.println("Top-up received...");
    Serial.println();
  }

  // Communicate with card
  for (byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF;
  }
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "82 D3 B3 3C") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    Serial.println();

    Serial.println("Reading from Data Block...");
    String string = "";
    ReadDataFromBlock(BLOCK, READ_DATA);
    Serial.print("\n");
    Serial.print("Data in Block:");
    Serial.print(BLOCK);
    Serial.print(" --> ");
    for (int j = 0 ; j < 16 ; j++)
    {
      Serial.write(READ_DATA[j]);
      string += (char)READ_DATA[j];
    }
    Serial.print("\nCurrent sum: ");
    CURRENT = string.toInt();
    Serial.print(CURRENT);
    Serial.print("\n\n");

    if (TOPUP > 0)
    {
      CURRENT += TOPUP;
      TOPUP = 0;
      string = "";
      for (int j = 0 ; j < 16 - String(CURRENT).length() ; j++)
      {
        string += '0';
      }
      for (int j = 0 ; j < String(CURRENT).length() ; j++)
      {
        string += String(CURRENT).charAt(j);
      }
      for (int j = 0 ; j < 16 ; j++)
      {
        WRITE_DATA[j] = (byte)string[j];
      }

      Serial.println("Writing to Data Block...");
      WriteDataToBlock(BLOCK, WRITE_DATA);
      Serial.print("\n");
    }
    else {
      if (CURRENT - 10 >= 0) {
        CURRENT -= 10;
        string = "";
        for (int j = 0 ; j < 16 - String(CURRENT).length() ; j++)
        {
          string += '0';
        }
        for (int j = 0 ; j < String(CURRENT).length() ; j++)
        {
          string += String(CURRENT).charAt(j);
        }
        for (int j = 0 ; j < 16 ; j++)
        {
          WRITE_DATA[j] = (byte)string[j];
        }

        Serial.println("Writing to Data Block...");
        WriteDataToBlock(BLOCK, WRITE_DATA);
        Serial.print("\n");
      }
      else
        Serial.println("Insufficient funds :(\n\n");
    }

    mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
    Serial.println();

    Serial.print("\nNew sum: ");
    Serial.print(CURRENT);
    Serial.print("\n\n");

    BLU.println("#" + String(CURRENT) + " RON");
    Serial.println("#" + String(CURRENT) + " RON");

    delay(3000);
    Serial.println("Approach your card to the reader...");
  }

  else   {
    Serial.println(" Access denied");
    delay(3000);
  }
}


void WriteDataToBlock(int blockNum, byte blockData[])
{
  /* Authenticating the desired data block for write access using Key A */
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print("Authentication failed for Write: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else
  {
    Serial.println("Authentication success");
  }


  /* Write data to the block */
  status = mfrc522.MIFARE_Write(blockNum, blockData, 16);
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print("Writing to Block failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else
  {
    Serial.println("Data was written into Block successfully");
  }

}

void ReadDataFromBlock(int blockNum, byte readBlockData[])
{
  /* Authenticating the desired data block for Read access using Key A */
  byte status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &key, &(mfrc522.uid));

  if (status != MFRC522::STATUS_OK)
  {
    Serial.print("Authentication failed for Read: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else
  {
    Serial.println("Authentication success");
  }

  /* Reading data from the Block */
  status = mfrc522.MIFARE_Read(blockNum, readBlockData, &bufferLen);
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print("Reading failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else
  {
    Serial.println("Block was read successfully");
  }

}
