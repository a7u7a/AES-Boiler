if (digitalRead(usrBtnPin) == HIGH)
{
    if (usrBtnState == false)
    {
        usrBtnState = true;
        buttonTimer = millis();
    }
    if ((millis() - buttonTimer > longPressTime) && (longPressActive == false))
    {
        longPressActive = true;
        bugMode = !bugMode;
     
    }
}
else
{
    if (usrBtnState == true)
    {
        if (longPressActive == true)
        {
            longPressActive = false;
        }
        else
        {
            LED2State = !LED2State;
            digitalWrite(LED2, LED2State);
        }
        usrBtnState = false;
    }
}
