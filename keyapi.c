#include "keyapi.h"

void InitKey(Key_TypeDef *key, GPIO_TypeDef *GPIOx, uint16_t pin, Key_Delays delays, Key_IDs IDs, Key_Flags flags)
{
   /* Initialising counters with 0 */
   key->counters.betweenClicks = 0;
   key->counters.clicks = 0;
   key->counters.pressTime = 0;

   key->GPIOx = GPIOx;
   key->pin = pin;

   key->delays = delays;
   key->IDs = IDs;
   key->flags = flags;

   key->flags.bits.longPressEvent = delays.longPressEvent > 0 ? flags.bits.longPressEvent : 0;
}

int8_t HandleKey(Key_TypeDef *key)
{
   if ((key->GPIOx->IDR & key->pin) == key->flags.bits.activeLevel)
   {
      key->counters.pressTime++;

      if (key->flags.bits.longPressEvent)
      {
         if (key->counters.pressTime == key->delays.longPressEvent) // long press event
         {
            if (key->flags.bits.autorepeat)
               key->counters.pressTime = key->delays.longPressEvent - key->delays.betweenRepeats;

            key->flags.bits.pressed = 1;
            return (int8_t)key->IDs.longPressEvent; // return ID of long press for this key
         }
         else
            return -1;
      }
      else
         return -1;
   }
   else
   {
      // Check if time pressed is enough and key wasn't pressed before to call short press event
      if ((key->counters.pressTime >= key->delays.shortPress) && !(key->flags.bits.pressed))
      {
         key->counters.pressTime = 0;
         key->flags.bits.pressed = 1;
         key->counters.clicks++;

         if (key->flags.bits.multiClickEvent)
         {
            if (key->counters.clicks > 2)
               return (int8_t)key->IDs.multiClick;
         }
         else
            return (int8_t)key->IDs.shortPress;
      }
      else
      {
         key->counters.pressTime = 0;
         key->flags.bits.pressed = 0;

         // if delay exceeds maximum, then reset clicks
         if (++key->counters.betweenClicks >= key->delays.betweenClicks)
            key->counters.clicks = 0;

         return -1;
      }
   }

   return -1;
}