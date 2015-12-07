using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesignPatterns
{
    /// <summary>
    /// Маг.
    /// Клонирует юнитов своего стека, у которых определен интерфейс IClonableUnit,
    /// с тем же значением Health. 
    /// </summary>
    class MageUnit : LightUnit, ISpecialAction
    {
        public override String Name
        {
            get
            {
                return "Mage";
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
                return 400;
            }
        }

        public MageUnit(Int32 health = 100) : base(health) { }

        #region ISpecialAction

        public Int32 Range         { get { return 3; } }
        public Int32 Probability   { get { return 40; } }
        public Int32 Power         { get { return 0; } }

        public ActionTarget Target { get { return ActionTarget.Ally; } }

        public Boolean IsCompatibleType(Unit target)
        {
            return target is IClonableUnit;
        }

        /// <summary>
        /// Клонирование юнита.
        /// </summary>
        /// <param name="target">Юнит, допускающий клонирование.</param>
        /// <returns>Объект, инкапсулирующий прототип и клонированный юнит.</returns>
        public UnitKeeper SpecialAction(Unit target)
        {
            IClonableUnit prototype = target as IClonableUnit;

            if (prototype == null)
                throw new ArgumentException("Target is supposed to be an IClonableUnit unit.");

            UnitKeeper targetKeeper = prototype.MagicallyClone();

            targetKeeper.Log.ObjectName = target.Name;
            targetKeeper.Log.Action = "cloned";
            targetKeeper.Log.SubjectName = this.Name.ToLower();

            return targetKeeper;
        }

        #endregion

        public override string ToString()
        {
            return String.Format("M({0})", Health);
        }
    }
}
