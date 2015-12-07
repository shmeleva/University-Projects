using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesignPatterns
{
    /// <summary>
    /// Лекарь.
    /// Лечит юнитов своего стека, у которых определен интерфейс ICanBeHealed,
    /// в радиусе 3 клеток.
    /// </summary>
    class HealerUnit : LightUnit, ICanBeHealed, ISpecialAction
    {
        public override String Name
        {
            get
            {
                return "Healer";
            }
        }

        public override Int32 Attack
        {
            get
            {
                return 50;
            }
        }
        public override Int32 Defence
        {
            get
            {
                return 15;
            }
        }
        public override Int32 Cost
        {
            get
            {
                return 300;
            }
        }

        public override int Health
        {
            get
            {
                return base.Health;
            }
            protected set
            {
                _health = value > 0 ? (value < MaxHealth ? value : MaxHealth) : 0;
            }
        }

        public HealerUnit(Int32 health = 100)
        {
            Health = MaxHealth = health;
        }

        #region ICanBeHealed

        public Int32 MaxHealth
        {
            get;
            protected set;
        }

        public UnitKeeper Heal(Int32 healthPoints)
        {
            Unit clone = this.CloneModifiedUnit(healthPoints);
            return new UnitKeeper(this, clone, null);
        }

        #endregion

        #region ISpecialAction

        public Int32 Range
        {
            get
            {
                return 3;
            }
        
        }
        public Int32 Probability
        {
            get
            {
                return 60;
            }
        }
        public Int32 Power
        {
            get
            {
                return 5;
            }
        }
        public ActionTarget Target
        {
            get
            {
                return ActionTarget.Ally;
            }
        }

        public Boolean IsCompatibleType(Unit target)
        {
            return target is ICanBeHealed;
        }

        /// <summary>
        /// Исцеление юнита.
        /// </summary>
        /// <param name="target">Юнит, допускающий лечение.</param>
        /// <returns>Объект, инкапсулирующий состояния юнита до и после исцеления.</returns>
        public UnitKeeper SpecialAction(Unit target)
        {
            ICanBeHealed patient = target as ICanBeHealed;

            if (patient == null)
                throw new ArgumentException("Target is supposed to be an ICanBeHealed unit.");

            UnitKeeper targetKeeper = patient.Heal(Power);

            targetKeeper.Log.ObjectName = target.Name;
            targetKeeper.Log.Action = "healed";
            targetKeeper.Log.SubjectName = this.Name.ToLower();

            return targetKeeper;
        }

        #endregion

        public override string ToString()
        {
            return String.Format("H({0})", Health);
        }
    }
}
