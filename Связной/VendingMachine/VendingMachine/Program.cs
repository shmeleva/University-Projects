using System;
using VendingMachine.Products;
using VendingMachine.Finance;
using System.Collections;
using VendingMachine.Roles;

namespace VendingMachine
{
    class Program
    {
        static void Main(string[] args)
        {
            var customer = new Customer(ProjectDefaults.CustomerBalance);

            var machine = new VendingMachineSeller(ProjectDefaults.Menu,
                ProjectDefaults.VendingMachineBalance);

            var app = new ConsoleApplication(customer, machine);
            app.Main.Execute();
        }
    }
}
