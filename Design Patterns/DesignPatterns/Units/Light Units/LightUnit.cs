using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesignPatterns
{
    /// <summary>
    /// Легкий юнит.
    /// </summary>
    abstract class LightUnit : Unit
    {
        sealed public override String Type
        {
            get
            {
                return "Легкий";
            } 
        }

        protected LightUnit(int healthPoints = 100) : base(healthPoints) { }
    }
}
