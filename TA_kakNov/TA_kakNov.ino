#include <Fuzzy.h>

// Instantiating a Fuzzy object
Fuzzy *fuzzy = new Fuzzy();

void setup() {
  // Set the Serial output
  Serial.begin(9600);

  // Instantiating a FuzzyInput object
  FuzzyInput *tinggi_gelas = new FuzzyInput(1);

  FuzzySet *pendek = new FuzzySet(6, 6, 8, 9);
  tinggi_gelas->addFuzzySet(pendek);
  FuzzySet *sedang = new FuzzySet(8, 9, 11, 12);
  tinggi_gelas->addFuzzySet(sedang);
  FuzzySet *agak_tinggi = new FuzzySet(11, 12, 14, 15);
  tinggi_gelas->addFuzzySet(agak_tinggi);
  FuzzySet *tinggi = new FuzzySet(14, 15, 18, 18);
  tinggi_gelas->addFuzzySet(tinggi);

  fuzzy->addFuzzyInput(tinggi_gelas);

  // Instantiating a FuzzyOutput objects
  FuzzyOutput *duration = new FuzzyOutput(1);

  FuzzySet *slow = new FuzzySet(14000, 15000, 17000, 17000);
  duration->addFuzzySet(slow);
  FuzzySet *middle = new FuzzySet(11000, 12000, 13000, 14000);
  duration->addFuzzySet(middle);
  FuzzySet *fast = new FuzzySet(8000, 9000, 10000, 11000);
  duration->addFuzzySet(fast);
  FuzzySet *very_fast = new FuzzySet(6000, 6000, 7000, 8000);
  duration->addFuzzySet(very_fast);

  fuzzy->addFuzzyOutput(duration);

  // Building FuzzyRule "IF tinggi_gelas = pendek THEN duration = slow"
  // Jika tinggi gelas 6-8 (pendek) maka output durasi = 6000 ms(very_fast)
  FuzzyRuleAntecedent *if_gelas_is_pendek = new FuzzyRuleAntecedent();
  if_gelas_is_pendek->joinSingle(pendek);
  FuzzyRuleConsequent *then_duration_very_fast = new FuzzyRuleConsequent();
  then_duration_very_fast->addOutput(very_fast);

  FuzzyRule *Rule01 = new FuzzyRule(1, if_gelas_is_pendek, then_duration_very_fast);
  fuzzy->addFuzzyRule(Rule01);

  // Building FuzzyRule "IF tinggi_gelas = sedang THEN duration = middle"
  // Jika tinggi gelas 9-11 (sedang) maka output durasi = 9000 ms (fast)
  FuzzyRuleAntecedent *if_Gelas_is_sedang = new FuzzyRuleAntecedent();
  if_Gelas_is_sedang->joinSingle(sedang);
  FuzzyRuleConsequent *then_duration_fast = new FuzzyRuleConsequent();
  then_duration_fast->addOutput(fast);

  FuzzyRule *Rule02 = new FuzzyRule(2, if_Gelas_is_sedang, then_duration_fast);
  fuzzy->addFuzzyRule(Rule02);

  // Building FuzzyRule "IF tinggi_gelas = agak tinggi THEN duration = fast"
  // Jika tinggi gelas 12-14 (agak_tinggi) maka output durasi = 12000 ms (middle)
  FuzzyRuleAntecedent *if_gelas_is_agak_tinggi = new FuzzyRuleAntecedent();
  if_gelas_is_agak_tinggi->joinSingle(agak_tinggi);
  FuzzyRuleConsequent *then_dration_middle = new FuzzyRuleConsequent();
  then_dration_middle->addOutput(middle);

  FuzzyRule *Rule03 = new FuzzyRule(3, if_gelas_is_agak_tinggi, then_dration_middle);
  fuzzy->addFuzzyRule(Rule03);

  // Building FuzzyRule "IF tinggi_gelas = tinggi THEN duration =very_fast"
  // Jika tinggi gelas 15-17(tinggi) maka output durasi = 15000 ms (slow)
  FuzzyRuleAntecedent *if_gelas_is_tinggi = new FuzzyRuleAntecedent();
  if_gelas_is_tinggi->joinSingle(tinggi);
  FuzzyRuleConsequent *then_dration_slow = new FuzzyRuleConsequent();
  then_dration_slow->addOutput(slow);

  FuzzyRule *Rule04 = new FuzzyRule(4, if_gelas_is_tinggi, then_dration_slow);
  fuzzy->addFuzzyRule(Rule04);

}

void loop() {
  // Getting a random value
  int input = 8;

  Serial.println(input);
  // Set the random value as an input
  fuzzy->setInput(1, input);
  // Running the Fuzzification
  fuzzy->fuzzify();
  // Running the Defuzzification
  float output = fuzzy->defuzzify(1);
  // Printing something
  Serial.println("Result: ");
  Serial.print("\t\t\tduration: ");
  Serial.println(output);
  // wait 12 seconds
  delay(1000);
}
