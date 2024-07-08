#include <Fuzzy.h>

// Initialization a Fuzzy object
Fuzzy *fuzzy = new Fuzzy();

//Input Suhu
FuzzySet *uDingin = new FuzzySet(0, 0, 15, 17);
FuzzySet *uSejuk = new FuzzySet(15, 16, 17, 20);
FuzzySet *uNormal = new FuzzySet(21, 23, 24, 26);
FuzzySet *uPanas = new FuzzySet(24, 26, 27, 29);
FuzzySet *uSangatPanas = new FuzzySet(26, 29, 30, 30);

//Input Kelembapan
FuzzySet *uSangatKering = new FuzzySet(0, 0, 75, 80);
FuzzySet *uKering = new FuzzySet(75, 79, 81, 85);
FuzzySet *uLembab = new FuzzySet(80, 84, 86, 90);
FuzzySet *uBasah = new FuzzySet(85, 89, 91, 95);
FuzzySet *uSangatBasah = new FuzzySet(90, 95, 100, 100);

//Output Durasi Uap
FuzzySet *mati = new FuzzySet(0, 0, 500, 1000);
FuzzySet *sebentar = new FuzzySet(0, 0, 1000, 3000);
FuzzySet *sedang = new FuzzySet(3000, 4500, 5500, 7000);
FuzzySet *lama = new FuzzySet(5500, 6500, 7500, 10000);
FuzzySet *sangatLama = new FuzzySet(7500, 10000, 11000, 11000);

FuzzySet *vSlow = new FuzzySet(20, 20, 40, 60);
FuzzySet *slow = new FuzzySet(40, 55, 65, 80);
FuzzySet *medium = new FuzzySet(60, 75, 85, 100);
FuzzySet *high = new FuzzySet(80, 95, 105, 110);
FuzzySet *veryHigh = new FuzzySet(110, 115, 120, 120);

void setup() {

  // Set the Serial output
  Serial.begin(9600);

  // Instantiating a FuzzyInput object
  FuzzyInput *suhu = new FuzzyInput(1);

  suhu->addFuzzySet(uDingin);
  suhu->addFuzzySet(uSejuk);
  suhu->addFuzzySet(uNormal);
  suhu->addFuzzySet(uPanas);
  suhu->addFuzzySet(uSangatPanas);

  fuzzy->addFuzzyInput(suhu);

  // Instantiating a FuzzyInput object
  FuzzyInput *kelembapan = new FuzzyInput(2);

  kelembapan->addFuzzySet(uSangatKering);
  kelembapan->addFuzzySet(uKering);
  kelembapan->addFuzzySet(uLembab);
  kelembapan->addFuzzySet(uBasah);
  kelembapan->addFuzzySet(uSangatBasah);

  fuzzy->addFuzzyInput(kelembapan);

  // Instantiating a FuzzyOutput objects
  FuzzyOutput *time_Uap = new FuzzyOutput(1);

  time_Uap->addFuzzySet(mati);
  time_Uap->addFuzzySet(sebentar);
  time_Uap->addFuzzySet(sedang);
  time_Uap->addFuzzySet(lama);
  time_Uap->addFuzzySet(sangatLama);

  fuzzy->addFuzzyOutput(time_Uap);

  FuzzyOutput *speedFan = new FuzzyOutput(2);

  speedFan->addFuzzySet(vSlow);
  speedFan->addFuzzySet(slow);
  speedFan->addFuzzySet(medium);
  speedFan->addFuzzySet(high);
  speedFan->addFuzzySet(veryHigh);

  fuzzy->addFuzzyOutput(speedFan);

  // Rule1
  FuzzyRuleAntecedent *ifSuhuDinginAndKelembapanSangatKering = new FuzzyRuleAntecedent();
  ifSuhuDinginAndKelembapanSangatKering->joinWithAND(uDingin, uSangatKering);

  FuzzyRuleConsequent *thenTimeUapSedangSpeedFanVslow = new FuzzyRuleConsequent();
  thenTimeUapSedangSpeedFanVslow->addOutput(sedang);
  thenTimeUapSedangSpeedFanVslow->addOutput(vSlow);

  FuzzyRule *Rule1 = new FuzzyRule(1, ifSuhuDinginAndKelembapanSangatKering, thenTimeUapSedangSpeedFanVslow);
  fuzzy->addFuzzyRule(Rule1);

  // Rule2
  FuzzyRuleAntecedent *ifSuhuDinginAndKelembapanKering = new FuzzyRuleAntecedent();
  ifSuhuDinginAndKelembapanKering->joinWithAND(uDingin, uKering);

  FuzzyRuleConsequent *thenTimeUapIsSedangAndFanSpeedvSlow = new FuzzyRuleConsequent();
  thenTimeUapIsSedangAndFanSpeedvSlow->addOutput(sedang);
  thenTimeUapIsSedangAndFanSpeedvSlow->addOutput(vSlow);

  FuzzyRule *Rule2 = new FuzzyRule(2, ifSuhuDinginAndKelembapanKering, thenTimeUapIsSedangAndFanSpeedvSlow);
  fuzzy->addFuzzyRule(Rule2);

  // Rule3
  FuzzyRuleAntecedent *ifSuhuDinginAndKelembapanLembab = new FuzzyRuleAntecedent();
  ifSuhuDinginAndKelembapanLembab->joinWithAND(uDingin, uLembab);

  FuzzyRuleConsequent *thenTimeUapIsSebentarAndFanSpeedSlow = new FuzzyRuleConsequent();
  thenTimeUapIsSebentarAndFanSpeedSlow->addOutput(sebentar);
  thenTimeUapIsSebentarAndFanSpeedSlow->addOutput(slow);

  FuzzyRule *Rule3 = new FuzzyRule(3, ifSuhuDinginAndKelembapanLembab, thenTimeUapIsSebentarAndFanSpeedSlow);
  fuzzy->addFuzzyRule(Rule3);

  // Rule4
  FuzzyRuleAntecedent *ifSuhuDinginAndKelembapanBasah = new FuzzyRuleAntecedent();
  ifSuhuDinginAndKelembapanBasah->joinWithAND(uDingin, uBasah);

  FuzzyRuleConsequent *thenTimeUapMatiAndSpeedFanSlow = new FuzzyRuleConsequent();
  thenTimeUapMatiAndSpeedFanSlow->addOutput(mati);
  thenTimeUapMatiAndSpeedFanSlow->addOutput(slow);

  FuzzyRule *Rule4 = new FuzzyRule(4, ifSuhuDinginAndKelembapanBasah, thenTimeUapMatiAndSpeedFanSlow);
  fuzzy->addFuzzyRule(Rule4);

  // Rule5
  FuzzyRuleAntecedent *ifSuhuDinginAndKelembapanSangatBasah = new FuzzyRuleAntecedent();
  ifSuhuDinginAndKelembapanSangatBasah->joinWithAND(uDingin, uSangatBasah);

  FuzzyRuleConsequent *thenTimeUapMatiAndSpeedFanMedium = new FuzzyRuleConsequent();
  thenTimeUapMatiAndSpeedFanMedium->addOutput(mati);
  thenTimeUapMatiAndSpeedFanMedium->addOutput(medium);

  FuzzyRule *Rule5 = new FuzzyRule(5, ifSuhuDinginAndKelembapanSangatBasah, thenTimeUapMatiAndSpeedFanMedium);
  fuzzy->addFuzzyRule(Rule5);


 // Rule6
  FuzzyRuleAntecedent *ifSuhuSejukAndKelembapanSangatKering = new FuzzyRuleAntecedent();
  ifSuhuSejukAndKelembapanSangatKering->joinWithAND(uSejuk, uSangatKering);

  FuzzyRuleConsequent *thenTimeUapSedangAndSpeedFanVslow = new FuzzyRuleConsequent();
  thenTimeUapSedangAndSpeedFanVslow->addOutput(sedang);
  thenTimeUapSedangAndSpeedFanVslow->addOutput(vSlow);

  FuzzyRule *Rule6 = new FuzzyRule(6, ifSuhuSejukAndKelembapanSangatKering, thenTimeUapSedangAndSpeedFanVslow);
  fuzzy->addFuzzyRule(Rule6);

 // Rule7
  FuzzyRuleAntecedent *ifSuhuSejukAndKelembapanKering = new FuzzyRuleAntecedent();
  ifSuhuSejukAndKelembapanKering->joinWithAND(uSejuk, uKering);

  FuzzyRuleConsequent *thenTimeUapSedangAndSpeedFanSlow = new FuzzyRuleConsequent();
  thenTimeUapSedangAndSpeedFanSlow->addOutput(sedang);
  thenTimeUapSedangAndSpeedFanSlow->addOutput(slow);

  FuzzyRule *Rule7 = new FuzzyRule(7, ifSuhuSejukAndKelembapanKering, thenTimeUapSedangAndSpeedFanSlow);
  fuzzy->addFuzzyRule(Rule7);
 // Rule8
   FuzzyRuleAntecedent *ifSuhuSejukAndKelembapanLembab = new FuzzyRuleAntecedent();
  ifSuhuSejukAndKelembapanLembab->joinWithAND(uSejuk, uLembab);

  FuzzyRuleConsequent *thenTimeUapSebentarAndSpeedFanSlow = new FuzzyRuleConsequent();
  thenTimeUapSebentarAndSpeedFanSlow->addOutput(sebentar);
  thenTimeUapSebentarAndSpeedFanSlow->addOutput(slow);

  FuzzyRule *Rule8 = new FuzzyRule(8, ifSuhuSejukAndKelembapanLembab, thenTimeUapSebentarAndSpeedFanSlow);
  fuzzy->addFuzzyRule(Rule8);

 // Rule9
  FuzzyRuleAntecedent *ifSuhuSejukAndKelembapanBasah = new FuzzyRuleAntecedent();
  ifSuhuSejukAndKelembapanBasah->joinWithAND(uSejuk, uBasah);

  FuzzyRuleConsequent *thenTimeUapMatiAndSpeedfanMedium = new FuzzyRuleConsequent();
  thenTimeUapMatiAndSpeedfanMedium->addOutput(mati);
  thenTimeUapMatiAndSpeedfanMedium->addOutput(medium);

  FuzzyRule *Rule9 = new FuzzyRule(9, ifSuhuSejukAndKelembapanBasah, thenTimeUapMatiAndSpeedfanMedium);
  fuzzy->addFuzzyRule(Rule9);

 // Rule10
    FuzzyRuleAntecedent *ifSuhuSejukAndKelembapanSangatBasah = new FuzzyRuleAntecedent();
 ifSuhuSejukAndKelembapanSangatBasah->joinWithAND(uSejuk, uSangatBasah);

  FuzzyRuleConsequent *thenTimeUapMatiAndSpeedFanHigh = new FuzzyRuleConsequent();
  thenTimeUapMatiAndSpeedFanHigh->addOutput(mati);
  thenTimeUapMatiAndSpeedFanHigh->addOutput(high);

  FuzzyRule *Rule10 = new FuzzyRule(10, ifSuhuSejukAndKelembapanSangatBasah, thenTimeUapMatiAndSpeedFanHigh);
  fuzzy->addFuzzyRule(Rule10);

 // Rule11
    FuzzyRuleAntecedent *ifSuhuNormalAndKelembapanSangatKering = new FuzzyRuleAntecedent();
  ifSuhuNormalAndKelembapanSangatKering->joinWithAND(uNormal, uSangatKering);

  FuzzyRuleConsequent *thenTimeUapLamaAndSpeedFanSlow = new FuzzyRuleConsequent();
  thenTimeUapLamaAndSpeedFanSlow ->addOutput(lama);
  thenTimeUapLamaAndSpeedFanSlow ->addOutput(slow);

  FuzzyRule *Rule11 = new FuzzyRule(11, ifSuhuNormalAndKelembapanSangatKering, thenTimeUapLamaAndSpeedFanSlow);
  fuzzy->addFuzzyRule(Rule11);

 // Rule12
  FuzzyRuleAntecedent *ifSuhuNormalAndKelembapanKering = new FuzzyRuleAntecedent();
  ifSuhuNormalAndKelembapanKering->joinWithAND(uNormal, uKering);

  FuzzyRuleConsequent *thenTimeUapLamaAndSpeedFanslow = new FuzzyRuleConsequent();
  thenTimeUapLamaAndSpeedFanslow ->addOutput(lama);
 thenTimeUapLamaAndSpeedFanslow ->addOutput(slow);

  FuzzyRule *Rule12 = new FuzzyRule(12, ifSuhuNormalAndKelembapanKering, thenTimeUapLamaAndSpeedFanslow);
  fuzzy->addFuzzyRule(Rule12);

 // Rule13
  FuzzyRuleAntecedent *ifSuhuNormalAndKelembapanLembab = new FuzzyRuleAntecedent();
  ifSuhuNormalAndKelembapanLembab->joinWithAND(uNormal, uLembab);

  FuzzyRuleConsequent *thenTimeUapSedangAndSpeedFanMedium = new FuzzyRuleConsequent();
  thenTimeUapSedangAndSpeedFanMedium->addOutput(sedang);
 thenTimeUapSedangAndSpeedFanMedium ->addOutput(medium);

  FuzzyRule *Rule13 = new FuzzyRule(13, ifSuhuNormalAndKelembapanLembab, thenTimeUapSedangAndSpeedFanMedium);
  fuzzy->addFuzzyRule(Rule13);

 // Rule14
  FuzzyRuleAntecedent *ifSuhuNormalAndKelembapanBasah = new FuzzyRuleAntecedent();
 ifSuhuNormalAndKelembapanBasah->joinWithAND(uNormal, uBasah);

  FuzzyRuleConsequent *thenTimeUapSebentarAndSpeedFanHigh = new FuzzyRuleConsequent();
  thenTimeUapSebentarAndSpeedFanHigh->addOutput(sebentar);
  thenTimeUapSebentarAndSpeedFanHigh ->addOutput(high);

  FuzzyRule *Rule14 = new FuzzyRule(14, ifSuhuNormalAndKelembapanBasah,thenTimeUapSebentarAndSpeedFanHigh);
  fuzzy->addFuzzyRule(Rule14);

 // Rule15
  FuzzyRuleAntecedent *ifSuhuNormalAndKelembapanSangatBasah = new FuzzyRuleAntecedent();
  ifSuhuNormalAndKelembapanSangatBasah->joinWithAND(uNormal, uSangatBasah);

  FuzzyRuleConsequent *thenTimeUapSebentarAndSpeedFanVeryhigh = new FuzzyRuleConsequent();
  thenTimeUapSebentarAndSpeedFanVeryhigh->addOutput(sebentar);
  thenTimeUapSebentarAndSpeedFanVeryhigh ->addOutput(veryHigh);

  FuzzyRule *Rule15 = new FuzzyRule(15, ifSuhuNormalAndKelembapanSangatBasah,thenTimeUapSebentarAndSpeedFanVeryhigh);
  fuzzy->addFuzzyRule(Rule15);

 // Rule16
   FuzzyRuleAntecedent *ifSuhuPanasAndKelembapanSangatKering = new FuzzyRuleAntecedent();
  ifSuhuPanasAndKelembapanSangatKering ->joinWithAND(uPanas, uSangatKering);

  FuzzyRuleConsequent *thenTimeUapSangatLamaAndSpeedFanMedium = new FuzzyRuleConsequent();
  thenTimeUapSangatLamaAndSpeedFanMedium ->addOutput(sangatLama);
  thenTimeUapSangatLamaAndSpeedFanMedium  ->addOutput(medium);

  FuzzyRule *Rule16 = new FuzzyRule(16,ifSuhuPanasAndKelembapanSangatKering ,thenTimeUapSangatLamaAndSpeedFanMedium);
  fuzzy->addFuzzyRule(Rule16);

 // Rule17
  FuzzyRuleAntecedent *ifSuhuPanasAndKelembapanKering = new FuzzyRuleAntecedent();
  ifSuhuPanasAndKelembapanKering ->joinWithAND(uPanas, uKering);

  FuzzyRuleConsequent *thenTimeUapSangatLamaandSpeedFanMedium = new FuzzyRuleConsequent();
  thenTimeUapSangatLamaandSpeedFanMedium ->addOutput(sangatLama);
  thenTimeUapSangatLamaandSpeedFanMedium  ->addOutput(medium);

  FuzzyRule *Rule17 = new FuzzyRule(17,ifSuhuPanasAndKelembapanKering ,thenTimeUapSangatLamaandSpeedFanMedium);
  fuzzy->addFuzzyRule(Rule17);

 // Rule18
   FuzzyRuleAntecedent *ifSuhuPanasAndKelembapanLembab = new FuzzyRuleAntecedent();
  ifSuhuPanasAndKelembapanLembab->joinWithAND(uPanas, uLembab);

  FuzzyRuleConsequent *thenTimeUapLamaAndSpeedFanHigh = new FuzzyRuleConsequent();
  thenTimeUapLamaAndSpeedFanHigh ->addOutput(lama);
  thenTimeUapLamaAndSpeedFanHigh ->addOutput(high);

  FuzzyRule *Rule18 = new FuzzyRule(18,ifSuhuPanasAndKelembapanLembab,thenTimeUapLamaAndSpeedFanHigh);
  fuzzy->addFuzzyRule(Rule18);

 // Rule19
    FuzzyRuleAntecedent *ifSuhuPanasAndKelembapanBasah = new FuzzyRuleAntecedent();
  ifSuhuPanasAndKelembapanBasah->joinWithAND(uPanas, uBasah);

  FuzzyRuleConsequent *thenTimeUapSedangAndSpeedFanVeryhigh = new FuzzyRuleConsequent();
  thenTimeUapSedangAndSpeedFanVeryhigh ->addOutput(sedang);
  thenTimeUapSedangAndSpeedFanVeryhigh ->addOutput(veryHigh);

  FuzzyRule *Rule19 = new FuzzyRule(19, ifSuhuPanasAndKelembapanBasah,thenTimeUapSedangAndSpeedFanVeryhigh);
  fuzzy->addFuzzyRule(Rule19);

 // Rule20
  FuzzyRuleAntecedent *ifSuhuPanasAndKelembapanSangatBasah = new FuzzyRuleAntecedent();
  ifSuhuPanasAndKelembapanSangatBasah->joinWithAND(uPanas, uSangatBasah);

  FuzzyRuleConsequent *thenTimeUapsebentarAndSpeedFanVeryhigh = new FuzzyRuleConsequent();
  thenTimeUapsebentarAndSpeedFanVeryhigh ->addOutput(sebentar);
  thenTimeUapsebentarAndSpeedFanVeryhigh ->addOutput(veryHigh);

  FuzzyRule *Rule20 = new FuzzyRule(20, ifSuhuPanasAndKelembapanSangatBasah,thenTimeUapsebentarAndSpeedFanVeryhigh);
  fuzzy->addFuzzyRule(Rule20);

 // Rule21
  FuzzyRuleAntecedent *ifSuhuSangatPanasAndKelembapanSangatKering = new FuzzyRuleAntecedent();
  ifSuhuSangatPanasAndKelembapanSangatKering->joinWithAND(uSangatPanas, uSangatKering);

  FuzzyRuleConsequent *thenTimeUapSangatLamaAndSpeedFanHigh = new FuzzyRuleConsequent();
  thenTimeUapSangatLamaAndSpeedFanHigh ->addOutput(sangatLama);
  thenTimeUapSangatLamaAndSpeedFanHigh ->addOutput(high);

  FuzzyRule *Rule21 = new FuzzyRule(21, ifSuhuSangatPanasAndKelembapanSangatKering,thenTimeUapSangatLamaAndSpeedFanHigh);
  fuzzy->addFuzzyRule(Rule21);

 // Rule22
   FuzzyRuleAntecedent *ifSuhuSangatPanasAndKelembapanKering = new FuzzyRuleAntecedent();
  ifSuhuSangatPanasAndKelembapanKering->joinWithAND(uSangatPanas, uKering);

  FuzzyRuleConsequent *thenTimeUapSangatlamaAndSpeedFanHigh = new FuzzyRuleConsequent();
  thenTimeUapSangatlamaAndSpeedFanHigh ->addOutput(sangatLama);
  thenTimeUapSangatlamaAndSpeedFanHigh ->addOutput(high);

  FuzzyRule *Rule22 = new FuzzyRule(22, ifSuhuSangatPanasAndKelembapanKering,thenTimeUapSangatlamaAndSpeedFanHigh);
  fuzzy->addFuzzyRule(Rule22);

 // Rule23
  FuzzyRuleAntecedent *ifSuhuSangatPanasAndKelembapanLembab = new FuzzyRuleAntecedent();
  ifSuhuSangatPanasAndKelembapanLembab->joinWithAND(uSangatPanas, uLembab);

  FuzzyRuleConsequent *thenTimeUapSangatLamaAndSpeedFanVeryhigh = new FuzzyRuleConsequent();
  thenTimeUapSangatLamaAndSpeedFanVeryhigh ->addOutput(sangatLama);
  thenTimeUapSangatLamaAndSpeedFanVeryhigh ->addOutput(veryHigh);

  FuzzyRule *Rule23 = new FuzzyRule(23, ifSuhuSangatPanasAndKelembapanLembab,thenTimeUapSangatLamaAndSpeedFanVeryhigh);
  fuzzy->addFuzzyRule(Rule23);

 // Rule24
  FuzzyRuleAntecedent *ifSuhuSangatPanasAndKelembapanBasah = new FuzzyRuleAntecedent();
  ifSuhuSangatPanasAndKelembapanBasah->joinWithAND(uSangatPanas, uBasah);

  FuzzyRuleConsequent *thenTimeuapSedangAndSpeedFanVeryhigh = new FuzzyRuleConsequent();
  thenTimeuapSedangAndSpeedFanVeryhigh ->addOutput(sedang);
  thenTimeuapSedangAndSpeedFanVeryhigh ->addOutput(veryHigh);

  FuzzyRule *Rule24 = new FuzzyRule(24, ifSuhuSangatPanasAndKelembapanBasah,thenTimeuapSedangAndSpeedFanVeryhigh);
  fuzzy->addFuzzyRule(Rule24);
 // Rule25

 FuzzyRuleAntecedent *ifSuhuSangatPanasAndKelembapanSangatBasah = new FuzzyRuleAntecedent();
  ifSuhuSangatPanasAndKelembapanSangatBasah->joinWithAND(uSangatPanas, uSangatBasah);

  FuzzyRuleConsequent *thenTimeUapSedangAndSpeedFanveryhigh = new FuzzyRuleConsequent();
  thenTimeUapSedangAndSpeedFanveryhigh ->addOutput(sedang);
  thenTimeUapSedangAndSpeedFanveryhigh ->addOutput(veryHigh);

  FuzzyRule *Rule25 = new FuzzyRule(25, ifSuhuSangatPanasAndKelembapanSangatBasah, thenTimeUapSedangAndSpeedFanveryhigh);
  fuzzy->addFuzzyRule(Rule25);
}

void loop() {

  uint8_t valueTermo = 0;  //Data Sensor Suhu
  uint8_t valueDHT = 0;    // Data Sensor Kelembapan
  uint8_t speed_fan = 0;   //Output kecepatan kipas
  uint16_t time_uap = 0;   //Output durasi uap

  if (Serial.available() > 0) {
    valueTermo = Serial.parseInt();
    Serial.read();  // menghapus tanda pemisah
    valueDHT = Serial.parseInt();
    Serial.println();  // membuat baris baru
  }
  // Set the dt_suhu value as an input fuzzy
  fuzzy->setInput(1, valueTermo);
  fuzzy->setInput(2, valueDHT);
  fuzzy->fuzzify();

  // Declare outtime_Uap for Defuzzification data
  time_uap = fuzzy->defuzzify(1);
  speed_fan = fuzzy->defuzzify(2);

  if (time_uap < 2000) {
    time_uap = 0;
  }
  if (valueTermo == 0 && valueDHT == 0){
    speed_fan = 0;
    time_uap = 0;
  }

  Serial.println("\n\n\nInput: ");
  Serial.print("\tSuhu: ");
  Serial.print(valueTermo);
  Serial.print(", Kelembapan: ");
  Serial.println(valueDHT);

  Serial.println("Result: ");
  Serial.print("\tTime Uap: ");
  Serial.print(time_uap);
  Serial.print(", Speed Fan : ");
  Serial.println(speed_fan);

  delay(1000);
}