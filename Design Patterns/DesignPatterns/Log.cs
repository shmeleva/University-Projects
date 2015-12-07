using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesignPatterns
{
    class Log
    {
        private String _objColor;
        private String _objName;
        private String _action;
        private String _subbColor;
        private String _subbName;

        public String ObjectColor
        {
            get
            {
                return _objColor;
            }
            set
            {
                if (String.IsNullOrEmpty(_objColor))
                {
                    _objColor = value;
                }
            }
        }
        public String ObjectName
        {
            get
            {
                return _objName;
            }
            set
            {
                if (String.IsNullOrEmpty(_objName))
                {
                    _objName = value;
                }
            }
        }
        public String Action
        {
            get
            {
                return _action;
            }
            set
            {
                if (String.IsNullOrEmpty(_action))
                {
                    _action = value;
                }
            }
        }
        public String SubjectColor
        {
            get
            {
                return _subbColor;
            }
            set
            {
                if (String.IsNullOrEmpty(_subbColor))
                {
                    _subbColor = value;
                }
            }
        }
        public String SubjectName
        {
            get
            {
                return _subbName;
            }
            set
            {
                if (String.IsNullOrEmpty(_subbName))
                {
                    _subbName = value;
                }
            }
        }

        public override string ToString()
        {
            //return String.Format("{0} was {1} by {2}.", ObjectName, Action.ToLower(), SubjectName.ToLower());
            return String.Format("{0} {1} was {2} by {3} {4}.", ObjectColor, ObjectName.ToLower(), Action.ToLower(), SubjectColor.ToLower(), SubjectName.ToLower());
        }
    }
}
