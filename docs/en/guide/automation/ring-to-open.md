# Ring To Open <Badge type="warning" text="Party Mode" />

The Doorman firmware features a Ring To Open automation, which was previously known as Party Mode.

### What does it do?
As the name implies, the entrance door will open automatically when someone rings the doorbell.
This feature is especially useful during a party, allowing guests to enter the building simply by ringing the bell.

### How do I use it?
You can enable or disable the Ring To Open automation using the switch in Home Assistant or by issuing the `Function Button` command.

#### Configuration Options:
::: info Confirmation (for entrance doors)
If you enable this setting, your apartment doorbell will ring each time you activate the Ring To Open automation via command (function button).
:::

::: info Delay (for each door)
This setting allows you to configure a delay for the door opener. If you set the delay to 60 seconds, it will randomly select a delay between 5 and 15 seconds whenever the automation is executed.
:::

::: info Timeout (for each door)
You can choose to disable the Ring To Open automation after 5 to 60 minutes, after a single ring, or never.
:::

::: info Display Status (global)
You can choose whether the status LED blinks while the automation is active.  
:::

::: info Toggle Trigger (for entrance doors)
You can decide which trigger will toggle the automation.  
:::

::: info Pattern Condition (for each door)
You can choose which doorbell pattern will trigger the automation.
:::