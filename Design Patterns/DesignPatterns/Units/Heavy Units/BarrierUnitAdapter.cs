using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SpecialUnits;

namespace DesignPatterns
{
    /// <summary>
    /// Барьер. Адаптер для GulyayGorod.
    /// </summary>
    class BarrierUnitAdapter : HeavyUnit
    {
        private GulyayGorod _adaptee;

        public override String Name
        {
            get
            {
                return "Barrier";
            }
        }

        public override Int32 Attack
        {
            get
            {
                return _adaptee.GetStrength();
            }
        }
        public override Int32 Defence
        {
            get
            {
                return _adaptee.GetDefence();
            }
        }
        public override Int32 Cost
        {
            get
            {
                return _adaptee.GetCost();
            }
        }

        public override Int32 Health
        {
            get
            {
                return _adaptee.AreDeath ? _adaptee.GetHealth() : 0;
            }
        }

        public BarrierUnitAdapter(Int32 health = 100) : base(health)
        {
            _adaptee = new GulyayGorod(_health, 45, 800);
        }

        public override UnitKeeper FightBack(int damage)
        {
            BarrierUnitAdapter changed = (BarrierUnitAdapter)this.CloneModifiedUnit(-damage);
            return new UnitKeeper(this, changed, null);
        }

        public override Unit CloneModifiedUnit(Int32 healthPoints)
        {
            return new BarrierUnitAdapter(_health + healthPoints);
        }

        public override string ToString()
        {
            return String.Format("B({0})", Health);
        }
    }
}
